#include "DialogEditor/FDialogAssetEditorApp.h"

#include "Asset/DialogAsset.h"
#include "DialogEditor/DialogAssetAppMode.h"
#include "DialogNode/DialogGraphNode.h"
#include "RuntimeNode/DialogGraphNodeRuntime.h"
#include "RuntimeGraph/DialogGraphPinRuntime.h"
#include "RuntimeGraph/DialogGraphRuntime.h"
#include "DialogGraph/DialogGraphSchema.h"
#include "DialogNode/DialogGraphEndNode.h"
#include "RuntimeNode/NodeInfo/DialogNodeInfo.h"
#include "DialogNode/DialogGraphStartNode.h"
#include "Kismet2/BlueprintEditorUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogDialogAssetEditorAppSub, Log, All);

const FName FDialogAssetEditorApp::DefaultMode("DialogAssetAppMode");

void FDialogAssetEditorApp::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

void FDialogAssetEditorApp::InitEditor(const EToolkitMode::Type mode,
	const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit)
{
	if (!ObjectToEdit) return;
	
	WorkingDialogAsset = Cast<UDialogAsset>(ObjectToEdit);
	WorkingDialogGraph = FBlueprintEditorUtils::CreateNewGraph(
		WorkingDialogAsset,
		NAME_None,
		UEdGraph::StaticClass(),
		UDialogGraphSchema::StaticClass()
		);
	WorkingDialogAsset->SetPreSaveCallback([this]() { OnWorkingAssetPreSave(); });
	TArray<UObject*> ObjectsToEdit;
	ObjectsToEdit.Add(ObjectToEdit);
	InitAssetEditor(
		mode,
		InitToolkitHost,
		TEXT("DialogAssetEditor"),
		FTabManager::FLayout::NullLayout,
		true,
		true,
		ObjectsToEdit);
	
	AddApplicationMode(DefaultMode, MakeShareable(new FDialogAssetAppMode(SharedThis(this))));
	SetCurrentMode(DefaultMode);
	UpdateEditorGraphFromWorkingAsset();
}

void FDialogAssetEditorApp::SetWorkingGraphUI(TSharedPtr<class SGraphEditor> InWorkingGraphEditor)
{
	WorkingGraphEditor = InWorkingGraphEditor;
}

void FDialogAssetEditorApp::SetSelectedNodeDetailsView(TSharedPtr<class IDetailsView> InSelectedNodeDetailsView)
{
	SelectedNodeDetailsView = InSelectedNodeDetailsView;
	SelectedNodeDetailsView->OnFinishedChangingProperties().AddRaw(this, &FDialogAssetEditorApp::OnNodeDetailsUpdated);
}

void FDialogAssetEditorApp::OnGraphSelectionChanged(const FGraphPanelSelectionSet& NewSelection)
{
	for (UObject* Obj: NewSelection)
	{
		if (UDialogGraphNodeBase* Node = Cast<UDialogGraphNodeBase>(Obj))
		{
			SelectedNode = Node;
			SelectedNodeDetailsView->SetObject(Node->GetNodeInfo());
			return;
		}
	}
	SelectedNodeDetailsView->SetObject(nullptr);
}

void FDialogAssetEditorApp::OnClose()
{
	UpdateWorkingAssetFromGraph();
	WorkingDialogGraph->RemoveOnGraphChangedHandler(GraphChangedDelegateHandle);
	FAssetEditorToolkit::OnClose();
}

void FDialogAssetEditorApp::OnNodeDetailsUpdated(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (WorkingGraphEditor)
	{
		if (SelectedNode)
		{
			SelectedNode->OnPropertiesChanged();
		}
		WorkingGraphEditor->NotifyGraphChanged();
	}
}

void FDialogAssetEditorApp::OnWorkingAssetPreSave()
{
	UpdateWorkingAssetFromGraph();
}

void FDialogAssetEditorApp::UpdateWorkingAssetFromGraph()
{
	if (!WorkingDialogAsset || !WorkingDialogGraph) return;
	
	UDialogGraphRuntime* RuntimeGraph = NewObject<UDialogGraphRuntime>(WorkingDialogAsset);
	WorkingDialogAsset->DialogGraph = RuntimeGraph;
	TArray<std::pair<FGuid, FGuid>> Connections;
	TMap<FGuid, UDialogGraphPinRuntime*> IdToPinMap;
	
	// iterate through all nodes and create corresponding runtime nodes
	for (UEdGraphNode* GraphNode : WorkingDialogGraph->Nodes)
	{
		UDialogGraphNodeRuntime* RuntimeNode = NewObject<UDialogGraphNodeRuntime>(RuntimeGraph);
		// record position
		RuntimeNode->Position = FVector2D(GraphNode->NodePosX, GraphNode->NodePosY);
		
		// iterate through all pins in this node and create corresponding runtime pins
		for (UEdGraphPin* GraphPin : GraphNode->Pins)
		{
			UDialogGraphPinRuntime* RuntimePin = NewObject<UDialogGraphPinRuntime>(RuntimeNode);
			// recording pin informations
			RuntimePin->PinId = GraphPin->PinId;
			RuntimePin->PinName = GraphPin->PinName;
			
			// record all connections
			if (GraphPin->HasAnyConnections() && GraphPin->Direction == EGPD_Output)
			{
				std::pair<FGuid, FGuid> Connection = std::make_pair(GraphPin->PinId, GraphPin->LinkedTo[0]->PinId);
				Connections.Add(Connection);
			}
			// record id to pin map
			IdToPinMap.Add(GraphPin->PinId, RuntimePin);
			// record input and output pins for the runtime nodes
			if (GraphPin->Direction == EGPD_Input)
			{
				RuntimeNode->InputPin = RuntimePin;
			}else
			{
				RuntimeNode->OutputPins.Add(RuntimePin);
			}
		}
		// set node type and node info
		UDialogGraphNodeBase* GraphNodeBase = Cast<UDialogGraphNodeBase>(GraphNode);
		check(GraphNodeBase);
		RuntimeNode->NodeType = GraphNodeBase->GetNodeType();
		RuntimeNode->NodeInfo = DuplicateObject(GraphNodeBase->GetNodeInfo(), RuntimeNode);
		
		RuntimeGraph->DialogNodes.Add(RuntimeNode);
	}
	
	// update all pins 
	for (std::pair<FGuid, FGuid> Connection : Connections)
	{
		UDialogGraphPinRuntime* FromPin = IdToPinMap[Connection.first];
		UDialogGraphPinRuntime* ToPin = IdToPinMap[Connection.second];
		FromPin->Connection = ToPin;
	}
	
}

void FDialogAssetEditorApp::UpdateEditorGraphFromWorkingAsset()
{
	if (!WorkingDialogAsset->DialogGraph)
	{
		WorkingDialogGraph->GetSchema()->CreateDefaultNodesForGraph(*WorkingDialogGraph);
		return;
	}
	TArray<std::pair<FGuid, FGuid>> Connections;
	TMap<FGuid, UEdGraphPin*> IdToPinMap;
	for (UDialogGraphNodeRuntime* RuntimeNode : WorkingDialogAsset->DialogGraph->DialogNodes)
	{
		UDialogGraphNodeBase* GraphNode = nullptr;
		if (RuntimeNode->NodeType == EDialogNodeType::Start)
		{
			GraphNode = NewObject<UDialogGraphStartNode>(WorkingDialogGraph);
		}else if (RuntimeNode->NodeType == EDialogNodeType::Dialog)
		{
			GraphNode = NewObject<UDialogGraphNode>(WorkingDialogGraph);
		}else if (RuntimeNode->NodeType == EDialogNodeType::End)
		{
			GraphNode = NewObject<UDialogGraphEndNode>(WorkingDialogGraph);
		}else
		{
			UE_LOG(LogDialogAssetEditorAppSub, Error, TEXT("Unknown node type %d"), RuntimeNode->NodeType);
			continue;
		}
		GraphNode->CreateNewGuid();
		GraphNode->NodePosX = RuntimeNode->Position.X;
		GraphNode->NodePosY = RuntimeNode->Position.Y;
		if (UDialogNodeInfoBase* NodeInfo = RuntimeNode->NodeInfo)
		{
			GraphNode->SetNodeInfo(DuplicateObject(NodeInfo, RuntimeNode));
		}else
		{
			GraphNode->InitNodeInfo(GraphNode);
		}
		if (UDialogGraphPinRuntime* InputPin = RuntimeNode->InputPin){
			UEdGraphPin* GraphPin = GraphNode->CreateDialogPin(EGPD_Input, InputPin->PinName);
			GraphPin->PinId = InputPin->PinId;
			IdToPinMap.Add(InputPin->PinId, GraphPin);
			if (InputPin->Connection)
			{
				Connections.Add(std::make_pair(InputPin->PinId, InputPin->Connection->PinId));
			}
		}
		for (UDialogGraphPinRuntime* Pin : RuntimeNode->OutputPins)
		{
			UEdGraphPin* GraphPin = GraphNode->CreateDialogPin(EGPD_Output, Pin->PinName);
			GraphPin->PinId = Pin->PinId;
			IdToPinMap.Add(Pin->PinId, GraphPin);
			if (Pin->Connection)
			{
				Connections.Add(std::make_pair(Pin->PinId, Pin->Connection->PinId));
			}
			
		}
		WorkingDialogGraph->AddNode(GraphNode, true, true);
	}
	
	for (std::pair<FGuid,FGuid> Connection: Connections)
	{
		UEdGraphPin* FromPin = IdToPinMap[Connection.first];
		UEdGraphPin* ToPin = IdToPinMap[Connection.second];
		
		FromPin->LinkedTo.Add(ToPin);
		ToPin->LinkedTo.Add(FromPin);
		
	}
	
}


