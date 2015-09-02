#pragma once

#include "..\common\stdafx.h"


class FBXJointConverter {
public:
	/// <summary>
	/// Creates absolute markers from hierarchical skeleton
	/// </summary>
	/// <param name="pScene">FBX Scene</param>
	/// <param name="sNode">Skeleton root node</param>
	/// <param name="onlyCopyTPose">Don't animate markers, only copy T-Pose information</param>
	/// <return>Node representing marker Set</return>
	static FbxNode* toAbsoluteMarkers(FbxScene *pScene, FbxNode *sNode, bool globalTransformationEnable = true, bool enableVmarker = false, bool onlyCopyTPose = false);

	/// <summary>
	/// Creates hierarchical Skeleton from absolute markers
	/// </summary>
	/// <param name="pScene">FBX Scene</param>
	/// <param name="sNode">Skeleton used as a reference to build the hierarchy</param>
	/// <param name="markerSet">Markers to be used</param>
	/// <param name="newSkelName">Name of new skeleton to be created</param>
	/// <param name="keyTimeVec">Vector of key times to use, if empty, will be initialized from markers</param>
	/// <return>Pointer to our newly created skeleton</return>
	static FbxNode* fromAbsoluteMarkers(FbxScene *pScene, FbxNode *refNode, char *newSkelName, double fps, bool enableGlobalTransformation = true, bool enableVmarker = false, FbxNode *markerSet = NULL, std::vector<FbxTime> &keyVec = c_emptyVector);

	/// <summary>
	/// Extract key times from curve, and add them to provided vector
	/// </summary>
	/// <param name="srcCurve">Curve where key information is located</param>
	/// <param name="out_time">Vector where information will be written to</param>
	static void extractKeyTimesFromCurve(FbxAnimCurve *srcCurve, std::vector<FbxTime> &out_time);

	/// <summary>
	/// Find set of markers corresponding to a reference skeleton
	/// </summary>
	/// <param name="pScene">FBX Scene</param>
	/// <param name="refNode">Reference skeleton</param>
	/// <param name="tgtSkel">Skeleton root to receive hierarchy</param>
	static FbxNode* findMarkerSet(FbxScene *pScene, FbxNode *refNode);

	/// <summary>
	/// Find first set of markers in a scene
	/// </summary>
	/// <param name="pScene">FBX Scene</param>
	static FbxNode* findAnyMarkerSet(FbxScene *pScene);

	/// <summary>
	/// Find skeleton
	/// </summary>
	/// <param name="pScene">FBX Scene</param>
	/// <param name="refNode">Reference skeleton</param>
	/// <param name="tgtSkel">Skeleton root to receive hierarchy</param>
	static FbxNode* findAnySkeleton(FbxScene *pScene);


private:
	// Constant declaration
	static FbxAMatrix c_mIdentity;
	static char *c_setPrefix;
	static char *c_markerPrefix;
	static char *c_separator;
	static std::vector<FbxTime> c_emptyVector;

	// Method defintions


	/// <summary>
	/// Recursive function that animate psoitional markers created from skeleton
	/// </summary>
	/// <param name="pLayer">FBX Animation layer</param>
	/// <param name="kTime">Time for which corresponding key will be created</param>
	/// <param name="mSet">Marker Set</param>
	/// <param name="cNode">Current skeleton node</param>
	/// <param name="parentTrans">Parent transformation ( defaults to identity )</param>
	static void animatePositionalMarkers(FbxAnimLayer *pLayer, FbxTime kTime, FbxNode *mSet, FbxNode *cNode, bool enableGlobalTransformation, bool enableVirtualMarkers, FbxAMatrix parentTrans = c_mIdentity);

	/// <summary>
	/// Recursive function that animates skeleton from positonal markers. Used for curve reconstruction
	/// </summary>
	/// <param name="pLayer">FBX Animation layer</param>
	/// <param name="kTime">Time for which corresponding key will be created</param>
	/// <param name="mSet">Marker Set</param>
	/// <param name="tgtNode">Node to be animated</param>
	/// <param name="parentTrans">Parent transformation ( defaults to identity )</param>
	static bool animateJointsFromMarkers(FbxAnimLayer *pLayer, FbxTime kTime, FbxNode *mSet, FbxNode *tgtNode, bool enableGlobalTransformation, bool enableVirtualMarkers, FbxAMatrix parentTrans = c_mIdentity);

	/// <summary>
	/// Recursive function that adds a positional marker for each joint in the hierarchy
	/// </summary>
	/// <param name="pScene">FBX Scene</param>
	/// <param name="markerSet">Set to have markers added to, if NULL, markers are added to scene</param>
	/// <param name="cNode">Current joint node in the hierarchy</param>
	/// <param name="idGen">Used to assign ids to markers</param>
	/// <return>Next id to be used when marker is added</return>
	static int createMarkersHierarchy(FbxScene *pScene, FbxNode *markerSet, FbxNode *cNode, int idGen = 0, bool enableVmarker = false);





	/// <summary>
	/// Extract transformation matrix from joint node at a given time
	/// </summary>
	/// <param name="cNode">Fbx Node to have information extracted from</param>
	/// <param name="pLayer">FBX Animation layer</param>
	/// <param name="kTime">Time for whic transformaiton is acquired</param>
	/// <return>Transformation matrix</return>
	static FbxAMatrix extractTransformationMatrix(FbxNode *cNode, FbxAnimLayer *pLayer, FbxTime kTime);

	/// <summary>
	/// Applies transformation matrix to marker node
	/// </summary>
	/// <param name="cNode">Fbx Node to have transformation applied to/param>
	/// <param name="pLayer">FBX Animation layer</param>
	/// <param name="kTime">Time for transformation</param>
	/// <param name="T">Transformation Matrix</param>
	/// <param name="forceMode">Boolean value that defines whether we force the creation of animation curves, in case they do not exist (defaults to true).</param>
	static void applyTransformationMatrix(FbxNode *cNode, FbxAnimLayer *pLayer, FbxTime kTime, FbxAMatrix &T, bool forceMode = true, FbxNode *refNode = NULL);


	/// <summary>
	/// Makes a copy of a skeleton hierarchy, without copying animation curves
	/// </summary>
	/// <param name="pScene">FBX Scene</param>
	/// <param name="oriSkel">Skeleton that will be copied</param>
	/// <param name="tgtSkel">Skeleton root to receive hierarchy</param>
	static void copySkeleton(FbxScene *pScene, FbxNode *oriSkel, FbxNode *tgtSkel);

	/// <summary>
	/// Find marker inside marker set
	/// </summary>
	/// <param name="mSet">Markert Set node</param>
	/// <param name="refNode">Reference skeleton</param>
	static FbxNode* findMarker(FbxNode *mSet, FbxNode *refNode);

	/// <summary>
	/// Find marker inside marker set
	/// </summary>
	/// <param name="mSet">Markert Set node</param>
	/// <param name="refName">Reference name for marker</param>
	static FbxNode* findMarker(FbxNode *mSet, const char *refName);



	/// <summary>
	/// Copy curve information from one node to another. Keys are not copied, only curves are initialized.
	/// </summary>
	/// <param name="pLayer">FBX Animation layer</param>
	/// <param name="srcNode">Node from which information is extracted</param>
	/// <param name="tgtNode">Node for which information is copied to</param>
	static void copyCurveInformation(FbxAnimLayer *pLayer, FbxNode *srcNode, FbxNode *tgtNode);

	
	/// <summary>
	/// Add virtual markers.
	/// </summary>
	static void addVirtualMarkerPosition(FbxAnimLayer *pLayer, FbxNode *markerNode, FbxScene *pScene);

	static void applyTransformationVirtualMarkers(FbxNode *cMarkerNode, FbxAnimLayer *pLayer, FbxTime kTime, FbxAMatrix cTransformation);

	static FbxMatrix createConvMatrix() {
		FbxMatrix conv;
		/*conv.SetRow(0, FbxVector4(1, 0, 0, 1));
		conv.SetRow(1, FbxVector4(0, 1, 0, 1));
		conv.SetRow(2, FbxVector4(0, 0, 0, 0));
		conv.SetRow(3, FbxVector4(0, 0, 0, 1));*/
		conv.SetRow(0, FbxVector4(1, 0, 0, 0));
		conv.SetRow(1, FbxVector4(0, 1, 0, 0));
		conv.SetRow(2, FbxVector4(0, 0, 0, 0));
		conv.SetRow(3, FbxVector4(1, 1, 0, 1));
		return conv;
	}

	static FbxAMatrix extractTransformationFromVirtualMarkers(FbxNode *tgtMarkerNode, FbxAnimLayer *pLayer, FbxTime kTime);

	static FbxDouble3 extractTransVectorFromCurves(FbxNode *node, FbxAnimLayer *pLayer, FbxTime kTime);

	static FbxMatrix createConvInverse() {
		FbxMatrix convInverse;
		/*convInverse.SetRow(0, FbxVector4(1, 0, 0, -1));
		convInverse.SetRow(1, FbxVector4(0, 1, 0, -1));
		convInverse.SetRow(2, FbxVector4(0, 0, 1, -1));
		convInverse.SetRow(3, FbxVector4(0, 0, 0, 1));*/
		convInverse.SetRow(0, FbxVector4(1, 0, 0, 0));
		convInverse.SetRow(1, FbxVector4(0, 1, 0, 0));
		convInverse.SetRow(2, FbxVector4(0, 0, 1, 0));
		convInverse.SetRow(3, FbxVector4(-1, -1, -1, 1));


		return convInverse;
	}
};