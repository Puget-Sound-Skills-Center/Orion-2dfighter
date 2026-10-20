// Copyright Epic Games, Inc. All Rights Reserved.

#include "PaperEditorShared/SpriteGeometryEditCommands.h"

#define LOCTEXT_NAMESPACE "SpriteEditor"

//////////////////////////////////////////////////////////////////////////
// FSpriteGeometryEditCommands

void FSpriteGeometryEditCommands::RegisterCommands()
{
	// Show toggles
	UI_COMMAND(SetShowNormals, "Normals", "Toggles display of vertex normals in the Preview Pane.", EUserInterfaceActionType::ToggleButton, FInputChord());

	// Geometry editing commands
	UI_COMMAND(DeleteSelection, "Delete", "Delete the selection.", EUserInterfaceActionType::Button, FInputChord(EKeys::Platform_Delete, EModifierKey::None));
	UI_COMMAND(SplitEdge, "Split", "Split edge.", EUserInterfaceActionType::Button, FInputChord(EKeys::Insert, EModifierKey::None));
	UI_COMMAND(AddBoxShape, "Add Box", "Adds a new box shape.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(AddHurtBox, "Add Hit Box", "Adds a new Hit Box .", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(AddGuardBox, "Add Guard Box", "Adds a new Guard Box .", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(AddTraceBox, "Add Trace Box", "Adds a new Trace Box .", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(AddCustomBox, "Add Custom Box", "Adds a new Custom Box .For Advanced Users:Highly Customizable", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ToggleAddPolygonMode, "Add Polygon", "Adds a new polygon shape.", EUserInterfaceActionType::ToggleButton, FInputChord());
	UI_COMMAND(AddCircleShape, "Add Circle", "Adds a new circle shape.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(SnapAllVertices, "Snap to pixel grid", "Snaps all vertices to the pixel grid.", EUserInterfaceActionType::Button, FInputChord());
}

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
