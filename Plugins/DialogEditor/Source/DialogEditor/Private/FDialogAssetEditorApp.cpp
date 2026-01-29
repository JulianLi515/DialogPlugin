#include "FDialogAssetEditorApp.h"

#include "DialogAsset.h"
#include "DialogAssetAppMode.h"
#include "DialogGraphNode.h"
#include "DialogGraphNodeRuntime.h"
#include "DialogGraphPinRuntime.h"
#include "DialogGraphRuntime.h"
#include "DialogGraphSchema.h"
#include "Kismet2/BlueprintEditorUtils.h"

const FName FDialogAssetEditorApp::DefaultMode("DialogAssetAppMode");

void FDialogAssetEditorApp::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

void FDialogAssetEditorApp::InitEditor(const EToolkitMode::Type mode,
	const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit)
{
	if (ObjectToEdit)
	{
		WorkingDialogAsset = Cast<UDialogAsset>(ObjectToEdit);
		WorkingDialogGraph = FBlueprintEditorUtils::CreateNewGraph(
			WorkingDialogAsset,
			NAME_None,
			UEdGraph::StaticClass(),
			UDialogGraphSchema::StaticClass()
			);
	}
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
	GraphChangedDelegateHandle = WorkingDialogGraph->AddOnGraphChangedHandler(FOnGraphChanged::FDelegate::CreateSP(this, &FDialogAssetEditorApp::OnGraphChanged));
}

void FDialogAssetEditorApp::OnClose()
{
	UpdateWorkingAssetFromGraph();
	WorkingDialogGraph->RemoveOnGraphChangedHandler(GraphChangedDelegateHandle);
	FAssetEditorToolkit::OnClose();
}

void FDialogAssetEditorApp::OnGraphChanged(const FEdGraphEditAction& EditAction)
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
	for (UEdGraphNode* Node : WorkingDialogGraph->Nodes)
	{
		UDialogGraphNodeRuntime* RuntimeNode = NewObject<UDialogGraphNodeRuntime>(RuntimeGraph);
		// record position
		RuntimeNode->Position = FVector2D(Node->NodePosX, Node->NodePosY);
		// iterate through all pins in this node and create corresponding runtime pins
		for (UEdGraphPin* Pin : Node->Pins)
		{
			UDialogGraphPinRuntime* RuntimePin = NewObject<UDialogGraphPinRuntime>(RuntimeNode);
			// recording pin informations
			RuntimePin->PinId = Pin->PinId;
			RuntimePin->PinName = Pin->PinName;
			
			// record all connections
			if (Pin->HasAnyConnections() && Pin->Direction == EGPD_Output)
			{
				std::pair<FGuid, FGuid> Connection = std::make_pair(Pin->PinId, Pin->LinkedTo[0]->PinId);
				Connections.Add(Connection);
			}
			// record id to pin map
			IdToPinMap.Add(Pin->PinId, RuntimePin);
			// record input and output pins for the runtime nodes
			if (Pin->Direction == EGPD_Input)
			{
				RuntimeNode->InputPins.Add(RuntimePin);
			}else
			{
				RuntimeNode->OutputPins.Add(RuntimePin);
			}
		}
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
	if (!WorkingDialogAsset->DialogGraph) return;
	TArray<std::pair<FGuid, FGuid>> Connections;
	TMap<FGuid, UEdGraphPin*> IdToPinMap;
	for (UDialogGraphNodeRuntime* Node : WorkingDialogAsset->DialogGraph->DialogNodes)
	{
		UDialogGraphNode* GraphNode = NewObject<UDialogGraphNode>(WorkingDialogGraph);
		GraphNode->CreateNewGuid();
		GraphNode->NodePosX = Node->Position.X;
		GraphNode->NodePosY = Node->Position.Y;
		for (UDialogGraphPinRuntime* Pin : Node->InputPins)
		{
			UEdGraphPin* GraphPin = GraphNode->CreateCustomPin(EGPD_Input, Pin->PinName);
			GraphPin->PinId = Pin->PinId;
			IdToPinMap.Add(Pin->PinId, GraphPin);
			if (Pin->Connection)
			{
				Connections.Add(std::make_pair(Pin->PinId, Pin->Connection->PinId));
			}
		}
		for (UDialogGraphPinRuntime* Pin : Node->OutputPins)
		{
			UEdGraphPin* GraphPin = GraphNode->CreateCustomPin(EGPD_Output, Pin->PinName);
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
	}
	
}
