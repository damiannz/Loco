//
//  ForwardDynamicsBody.h
//  Loco
//
//  Created on 30/10/13.
//
//

#ifndef __Loco__ForwardDynamicsBody__
#define __Loco__ForwardDynamicsBody__

#include <iostream>
#include <Ogre/OgreSharedPtr.h>

#include "DriveableBone.h"
#include "picojson.h"
#include <map>
#include "OgreBulletDynamicsRigidBody.h"

namespace Ogre {
	class SceneNode;
}

namespace OgreBulletDynamics {
	class DynamicsWorld;
}

namespace OgreBulletCollisions {
	class CollisionShape;
	class DebugLines;
	
}

class ForwardDynamicsBody
{
public:
	ForwardDynamicsBody( Ogre::SharedPtr<DriveableBone> parentBone, std::vector<Ogre::SharedPtr<DriveableBone> > childBones, Ogre::SceneNode* skeletonRootNode, OgreBulletDynamics::DynamicsWorld* world, const picojson::value& bodyDef );
	~ForwardDynamicsBody();
	
	/*! @brief Return the head position (== position of parent joint) in world space. */
	Ogre::Vector3 getHeadPositionWorld();
	/*! @brief Return the head position (== position of parent joint) in local space (CoM == (0,0,0)). */
	Ogre::Vector3 getHeadPositionLocal() { return mParentPositionLocal; }
	/*! @brief Return the tail position (== position of child joint) in world space. */
	Ogre::Vector3 getTailPositionWorld(std::string tailName="");
	/*! @brief Return the tail position (== position of child joint) in local space (CoM == (0,0,0)). */
	Ogre::Vector3 getTailPositionLocal(std::string tailName="");
	/*! @brief Return the orientation in world space. */
	Ogre::Quaternion getOrientationWorld();
	/*! @brief Return the orientation in local space (ie relative to the scene node that is the parent of this bone. */
	Ogre::Quaternion getOrientationLocal();
	/*! @brief Return the rest orientation (the orientation on init) in local space. */
	Ogre::Quaternion getParentRelativeRestOrientation() { return mRestOrientationLocal; }
	void setParentRelativeRestOrientationLocal(const Ogre::Quaternion& q) { mRestOrientationLocal =q; }
	/*! @brief Return the position of the center of mass in world space. */
	Ogre::Vector3 getCoMWorld();
	
	Ogre::Vector3 convertLocalToWorldPosition(const Ogre::Vector3& localPos) const { return mBody->getSceneNode()->convertLocalToWorldPosition(localPos); }
	Ogre::Vector3 convertWorldToLocalPosition(const Ogre::Vector3& worldPos) const { return mBody->getSceneNode()->convertWorldToLocalPosition(worldPos); }
	
	Ogre::Quaternion convertLocalToWorldOrientation(const Ogre::Quaternion& localOri) const { return mBody->getSceneNode()->convertLocalToWorldOrientation(localOri); }
	Ogre::Quaternion convertWorldToLocalOrientation(const Ogre::Quaternion& worldOri) const { return mBody->getSceneNode()->convertWorldToLocalOrientation(worldOri); }
	
/**
 This method returns the absolute velocity of a point that is passed in as a parameter. The point is expressed in local coordinates, and the
 resulting velocity will be expressed in world coordinates.
 */
	Ogre::Vector3 getAbsoluteVelocityForLocalPoint(const Ogre::Vector3& localPoint);
	
	std::string getName() { return mParentBoneName; }
	unsigned int getNumChildren() { return mChildPositionsLocal.size(); }
	std::string getAnyChildName();
	
	OgreBulletDynamics::RigidBody* getBody() { return mBody; }
	
	void debugDraw(OgreBulletCollisions::DebugLines* debugLines);
	
	void reset( Ogre::SharedPtr<DriveableBone> parentBone, Ogre::SceneNode* skeletonRootNode );
	
	// apply torque to this body and reset
	void applyTorque();
	void addImpulse( const Ogre::Vector3& impulse );

	float getMaxTorque() { return mMaxTorque; }
	
	float getMass();
	float getKp() { return mKp; }
	float getKd() { return mKd; }
	
private:
	
	void addTorque( const Ogre::Vector3& torque ) { mTorque += torque; }
	
	std::string mParentBoneName;
	float mKp, mKd, mMaxTorque;
	Ogre::Vector3 mTorqueScale;
	
	Ogre::Vector3 mTorque;
	
	OgreBulletCollisions::CollisionShape* mCollisionShape;
	OgreBulletDynamics::RigidBody* mBody;
	
	Ogre::Vector3 mParentPositionLocal; // position of the parent in local coordinates
	std::map<std::string,Ogre::Vector3> mChildPositionsLocal; // position of the child in local coordinates
	
	Ogre::Quaternion mRestOrientationLocal;
};


#endif /* defined(__Loco__ForwardDynamicsBody__) */
