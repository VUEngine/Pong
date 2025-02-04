////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////  THIS FILE WAS AUTO-GENERATED - DO NOT EDIT  ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// INCLUDES
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#include <Actor.h>
#include <BgmapSprite.h>
#include <Body.h>
#include <Box.h>
#include <ColliderLayers.h>
#include <InGameTypes.h>
#include <Mutator.h>
#include <Paddle.h>
#include <Texture.h>

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// DECLARATIONS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

extern uint32 PaddleActorPaddleTiles[];
extern uint16 PaddleActorPaddleMap[];

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// SPRITES
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

CharSetROMSpec PaddleSprite1CharsetSpec =
{
	// Number of CHARs in function of the number of frames to load at the same time
	2,

	// Whether it is shared or not
	true,

	// Whether the tiles are optimized or not
	true,

	// Tiles array
	PaddleActorPaddleTiles,

	// Frame offsets array
	NULL
};

TextureROMSpec PaddleSprite1TextureSpec =
{
	// Pointer to the char spec that the texture uses
	(CharSetSpec*)&PaddleSprite1CharsetSpec,

	// Pointer to the map array that defines how to use the tiles from the char set
	PaddleActorPaddleMap,

	// Horizontal size in tiles of the texture (max. 64)
	1,

	// Vertical size in tiles of the texture (max. 64)
	4,

	// padding for affine/hbias transformations
	{0, 0},

	// Number of frames that the texture supports
	1,

	// Palette index to use by the graphical data (0 - 3)
	0,

	// Flag to recyble the texture with a different map
	false,

	// Flag to vertically flip the image
	false,

	// Flag to horizontally flip the image
	false
};

BgmapSpriteROMSpec PaddleSprite1SpriteSpec =
{
	{
		// VisualComponent
		{
			// Component
			{
				// Allocator
				__TYPE(BgmapSprite),

				// Component type
				kSpriteComponent
			},

			// Array of animation functions
			(const AnimationFunction**)NULL
		},

		// Spec for the texture to display
		(TextureSpec*)&PaddleSprite1TextureSpec,

		// Transparency mode (__TRANSPARENCY_NONE, __TRANSPARENCY_EVEN or __TRANSPARENCY_ODD)
		__TRANSPARENCY_NONE,

		// Displacement added to the sprite's position
		{0, 0, 0, 0}
	},

	// Flag to indicate in which display to show the texture (__WORLD_ON, __WORLD_LON or __WORLD_RON)
	__WORLD_ON,

	// The display mode (__WORLD_BGMAP, __WORLD_AFFINE, __WORLD_OBJECT or __WORLD_HBIAS)
	// make sure to use the proper corresponding sprite type throughout the spec (BgmapSprite or ObjectSprite)
	__WORLD_BGMAP,
	
	// Pointer to affine/hbias manipulation function
	NULL
};

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// COLLIDERS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

ColliderROMSpec PaddleCollider1ColliderSpec = 
{
	// Collider
	{
		// Allocator
		__TYPE(Box),

		// Component type
		kColliderComponent
	},

	// Size (x, y, z)
	{8, 32, 16},

	// Displacement (x, y, z, p)
	{0, 0, 0, 0},

	// Rotation (x, y, z)
	{ __F_TO_FIX7_9(0.000f), __F_TO_FIX7_9(0.000f), __F_TO_FIX7_9(0.000f) },

	// Scale (x, y, z)
	{ __F_TO_FIX7_9(1.000f), __F_TO_FIX7_9(1.000f), __F_TO_FIX7_9(1.000f) },

	// If true this collider checks for collisions against other colliders
	false,

	// Layers in which I live
	kLayerPaddle,

	// Layers to ignore when checking for collisions
	kLayerAll
};

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// BODY
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

BodyROMSpec PaddleBodySpec =
{
	// Component
	{
		// Allocator
		__TYPE(Body),

		// Component type
		kPhysicsComponent
	},

	// Create body
	true,

	// Mass
	__F_TO_FIX10_6(0.550f),

	// Friction
	__F_TO_FIX10_6(0.250f),

	// Bounciness
	__F_TO_FIX10_6(1.000f),

	// Maximum velocity
	{ __I_TO_FIXED(0), __I_TO_FIXED(0), __I_TO_FIXED(0) },

	// Maximum speed
	__I_TO_FIX10_6(8),

	// Axises on which the body is subject to gravity
	__NO_AXIS,

	// Axises around which to rotate the owner when syncronizing with body
	__NO_AXIS
};

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// MUTATORS
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

MutatorROMSpec PaddleMutator1MutatorSpec =
{
	{
		// Allocator
		__TYPE(Mutator),

		// Component type
		kMutatorComponent
	},

	// Mutation target class
	class(Paddle),

	// Enabled
	true
};

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// ACTOR
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

ComponentSpec* const PaddleComponentSpecs[] = 
{
	(ComponentSpec*)&PaddleSprite1SpriteSpec,
	(ComponentSpec*)&PaddleCollider1ColliderSpec,
	(ComponentSpec*)&PaddleBodySpec,
	(ComponentSpec*)&PaddleMutator1MutatorSpec,
	NULL
};

ActorROMSpec PaddleActorSpec =
{
	// Class allocator
	__TYPE(Actor),

	// Component specs
	(ComponentSpec**)PaddleComponentSpecs,

	// Children specs
	NULL,

	// Extra info
	NULL,

	// Size
	// If 0, it is computed from the visual components if any
	{0, 0, 0},

	// Actor's in-game type
	kTypePaddle,

	// Animation to play automatically
	NULL
};
