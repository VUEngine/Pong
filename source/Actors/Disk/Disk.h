/*
 * VUEngine Showcase
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef DISK_H_
#define DISK_H_

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// INCLUDES
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#include <Actor.h>

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// CLASS' DECLARATION
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

/// Class Disk
///
/// Inherits from Actor
///
/// Implements a Pong disk.
mutation class Disk : Actor
{
	/// Default interger message handler for propagateMessage
	/// @param message: Propagated integer message
	/// @return True if the propagation must stop; false if the propagation must reach other containers
	override bool handlePropagatedMessage(int32 message);

	/// Process a newly detected collision by one of the component colliders.
	/// @param collisionInformation: Information struct about the collision to resolve
	/// @return True if the collider must keep track of the collision to detect if it persists and when it
	/// ends; false otherwise
	override bool collisionStarts(const CollisionInformation* collisionInformation);

	/// Update this instance's logic.
	override void update();

	/// Method to determine whether or not to synchronize the position with the remote's.
	/// @return True if must synchronize the position with the remote instance
	virtual bool mustSychronize();
}

#endif
