#pragma once

#ifndef __CSC_MATRIX__
#error "∑(っ°Д° ;)っ : require module"
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#include "csc_matrix.hpp"

#include "csc_end.h"
#ifdef __CSC_COMPILER_MSVC__
#endif

#include <nanoflann.hpp>
#include "csc_begin.h"

namespace CSC {
struct KDTreePointCloudAdaptor {
	RefBuffer<FLT32> mPointCloud ;
	csc_size_t mDimension ;
	csc_size_t mSize ;

public:
	csc_size_t kdtree_get_point_count () const {
		return mSize ;
	}

	FLT32 kdtree_distance (const FLT32 *pt ,csc_size_t idx ,csc_size_t size_) const {
		auto rax = FLT64 (0) ;
		for (auto &&i : iter (0 ,LENGTH (size_))) {
			const auto r1x = pt[i] - mPointCloud[idx * mDimension + i] ;
			rax += MathProc::square (r1x) ;
		}
		return FLT32 (rax) ;
	}

	FLT32 kdtree_get_pt (csc_size_t idx ,csc_size_t dim) const {
		if (dim >= mDimension)
			return 0 ;
		INDEX ix = INDEX (idx * mDimension + dim) ;
		return mPointCloud[ix] ;
	}

	template <class BBOX>
	BOOL kdtree_get_bbox (BBOX &) const {
		return false ;
	}
} ;

class KDTreeResultAdaptor {
public:
	using DistanceType = FLT32 ;
	using IndexType = INDEX ;

public:
	FLT32 mRadius ;
	Priority<IndexPair<FLT32>> mResult ;

public:
	implicit KDTreeResultAdaptor () = delete ;

	explicit KDTreeResultAdaptor (CREF<LENGTH> capacity ,CREF<FLT32> radius_) {
		mRadius = radius_ ;
		mResult = Priority<IndexPair<FLT32>> (capacity + 1) ;
		clear () ;
	}

	void clear () {
		mResult.clear () ;
	}

	csc_size_t size () const {
		return mResult.length () ;
	}

	csc_size_t empty () const {
		return mResult.empty () ;
	}

	BOOL full () const {
		return true ;
	}

	BOOL addPoint (FLT32 dist ,INDEX index) {
		if (dist >= mRadius)
			return true ;
		mResult.add ({-dist ,index}) ;
		if ifdo (TRUE) {
			if (!mResult.full ())
				discard ;
			mResult.take () ;
			mRadius = MathProc::min_of (mRadius ,-mResult[0].mItem) ;
		}
		return true ;
	}

	FLT32 worstDist () const {
		return mRadius ;
	}

	nanoflann::ResultItem<INDEX ,FLT32> worst_item () const {
		assume (mResult.length () > 0) ;
		nanoflann::ResultItem<INDEX ,FLT32> ret ;
		ret.first = mResult[0].mIndex ;
		ret.second = -mResult[0].mItem ;
		return move (ret) ;
	}
} ;

inline namespace {
using KDTreeDistance = nanoflann::L2_Simple_Adaptor<FLT32 ,KDTreePointCloudAdaptor ,FLT32 ,INDEX> ;
using KDTree = nanoflann::KDTreeSingleIndexAdaptor<KDTreeDistance ,KDTreePointCloudAdaptor ,3 ,INDEX> ;
} ;

struct PointCloudKDTreeLayout {
	KDTreePointCloudAdaptor mAdaptor ;
	Box<KDTree> mKDTree ;
} ;

class PointCloudKDTreeImplHolder final implement Fat<PointCloudKDTreeHolder ,PointCloudKDTreeLayout> {
public:
	void create (VREF<AutoRef<PointCloudKDTreeLayout>> that) const override {
		that = AutoRef<PointCloudKDTreeLayout>::make () ;
	}

	void initialize (CREF<Array<Pointer>> pointcloud) override {
		const auto r1x = address (pointcloud[0]) ;
		const auto r2x = pointcloud.step () / SIZE_OF<FLT32>::expr ;
		assume (inline_between (r2x ,1 ,4)) ;
		const auto r3x = pointcloud.size () * r2x ;
		self.mAdaptor.mPointCloud = RefBuffer<FLT32>::reference (r1x ,r3x) ;
		self.mAdaptor.mDimension = r2x ;
		self.mAdaptor.mSize = pointcloud.length () ;
		const auto r4x = nanoflann::KDTreeSingleIndexAdaptorParams () ;
		self.mKDTree = Box<KDTree>::make (VAL32 (r2x) ,self.mAdaptor ,r4x) ;
		self.mKDTree->buildIndex () ;
	}

	Array<INDEX> search (CREF<Vector> center ,CREF<LENGTH> neighbor) const override {
		assert (neighbor > 0) ;
		const auto r1x = Point3F (center.xyz ()) ;
		const auto r2x = nanoflann::SearchParameters (0 ,false) ;
		auto rax = KDTreeResultAdaptor (neighbor ,infinity) ;
		self.mKDTree->findNeighbors (rax ,(&r1x.mX) ,r2x) ;
		const auto r3x = inline_min (rax.mResult.length () ,neighbor) ;
		Array<INDEX> ret = Array<INDEX> (r3x) ;
		for (auto &&i : iter (0 ,r3x)) {
			INDEX ix = r3x - 1 - i ;
			ret[ix] = rax.mResult[0].mIndex ;
			rax.mResult.take () ;
		}
		return move (ret) ;
	}

	Array<INDEX> search (CREF<Vector> center ,CREF<LENGTH> neighbor ,CREF<FLT64> radius) const override {
		assert (neighbor > 0) ;
		const auto r1x = Point3F (center.xyz ()) ;
		const auto r2x = nanoflann::SearchParameters (0 ,false) ;
		auto rax = KDTreeResultAdaptor (neighbor ,FLT32 (MathProc::square (radius))) ;
		self.mKDTree->radiusSearchCustomCallback ((&r1x.mX) ,rax ,r2x) ;
		const auto r3x = inline_min (rax.mResult.length () ,neighbor) ;
		Array<INDEX> ret = Array<INDEX> (r3x) ;
		for (auto &&i : iter (0 ,r3x)) {
			INDEX ix = r3x - 1 - i ;
			ret[ix] = rax.mResult[0].mIndex ;
			rax.mResult.take () ;
		}
		return move (ret) ;
	}
} ;

static const auto mPointCloudKDTreecExternal = External<PointCloudKDTreeHolder ,PointCloudKDTreeLayout> (PointCloudKDTreeImplHolder ()) ;
} ;