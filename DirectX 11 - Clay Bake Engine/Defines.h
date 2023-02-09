#pragma once

#define EDIT_MODE false

#define DEBUG true

#define FPS_5 1.0f/5.0f
#define FPS_30 1.0f/30.0f
#define FPS_60 1.0f/60.0f
#define FPS_CAP FPS_60

#define FIXED_TIMESTEP 1.0f/50.0f

#define JSON_SCENE_ID "sceneID"
#define JSON_SCENE_BACKGROUND "backgroundImage"
#define JSON_SCENE_BACKGROUNDAUDIO "backgroundAudio"
#define JSON_SCENE_GAMEOBJECTS "gameObjects"
#define JSON_SCENE_STAGECOLLISION "stageCollision"

#define JSON_GO_NAME "name"
#define JSON_GO_TAG "tag"
#define JSON_GO_POSITION "position"
#define JSON_GO_ROTATION "rotation"
#define JSON_GO_SCALE "scale"
#define JSON_GO_COMPONENTS "components"

#define JSON_COMPONENT_CLASS "class"
#define JSON_COMPONENT_CONSTRUCTORS "constructors"

#define JSON_TAG_GAMEOBJECT "GameObject"
#define JSON_TAG_STAGECOLLISION "StageCollision"