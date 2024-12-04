#include "Animation.h"


void AnimationSequence::calcFrame(float t, int& frame, float& interpolationFact)
{
	interpolationFact = t * ticksPerSecond;
	frame = (int)floorf(interpolationFact);
	interpolationFact = interpolationFact - (float)frame;
	frame = min(frame, frames.size() - 1);
}


Matrix AnimationSequence::interpolateBoneToGlobal(Matrix* matrices, int baseFrame, float interpolationFact, Skeleton* skeleton, int boneIndex)
{
	Matrix scale = Matrix::Scaling(interpolate(frames[baseFrame].scales[boneIndex], frames[nextFrame(baseFrame)].scales[boneIndex], interpolationFact));
	Matrix rotation = interpolate(frames[baseFrame].rotations[boneIndex], frames[nextFrame(baseFrame)].rotations[boneIndex], interpolationFact).toRotationMatrix();
	Matrix translation = Matrix::Translation(interpolate(frames[baseFrame].positions[boneIndex], frames[nextFrame(baseFrame)].positions[boneIndex], interpolationFact));
	Matrix local = translation * rotation * scale;
	if (skeleton->bones[boneIndex].parentIndex > -1)
	{
		Matrix global = matrices[skeleton->bones[boneIndex].parentIndex] * local;
		return global;
	}
	return local;
}

void AnimationInstance::update(std::string name, float dt) {
	if (name == currentAnimation) {
		t += dt;
	}
	else {
		currentAnimation = name;  t = 0;
	}
	if (animationFinished() == true) { resetAnimationTime(); }
	int frame = 0;
	float interpolationFact = 0;
	animation->calcFrame(name, t, frame, interpolationFact);
	for (int i = 0; i < animation->skeleton.bones.size(); i++)
	{
		matrices[i] = animation->interpolateBoneToGlobal(name, matrices, frame, interpolationFact, i);
	}
	animation->calcFinalTransforms(matrices);
}