// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/MeshComponent.h"
#include "ProceduralMeshComponentX.generated.h"

USTRUCT(BlueprintType)
struct FProceduralMeshVertex
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, Category = Triangle)
    FVector Position;

    UPROPERTY(EditAnywhere, Category = Triangle)
    FColor Color;

};

USTRUCT(BlueprintType)
struct FProceduralMeshTriangle
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, Category = Triangle)
    FProceduralMeshVertex Vertex0;

    UPROPERTY(EditAnywhere, Category = Triangle)
    FProceduralMeshVertex Vertex1;

    UPROPERTY(EditAnywhere, Category = Triangle)
    FProceduralMeshVertex Vertex2;
};

/** Component that allows you to specify custom triangle mesh geometry */
UCLASS(editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = Rendering)
class UProceduralMeshComponentX : public UMeshComponent, public IInterface_CollisionDataProvider
{
    GENERATED_UCLASS_BODY()

public:
    /** Set the geometry to use on this triangle mesh */
    UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
    bool SetProceduralMeshTriangles(const TArray<FProceduralMeshTriangle>& Triangles);

    /** Description of collision */
    UPROPERTY(BlueprintReadOnly, Category = "Collision")
    class UBodySetup * ModelBodySetup;

    // Begin Interface_CollisionDataProvider Interface
    virtual bool GetPhysicsTriMeshData(struct FTriMeshCollisionData* CollisionData, bool InUseAllTriData) override;
    virtual bool ContainsPhysicsTriMeshData(bool InUseAllTriData) const override;
    virtual bool WantsNegXTriMesh() override
    {
        return false;
    }
    // End Interface_CollisionDataProvider Interface

    // Begin UPrimitiveComponent interface.
    virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
    virtual class UBodySetup* GetBodySetup() override;
    // End UPrimitiveComponent interface.

    // Begin UMeshComponent interface.
    virtual int32 GetNumMaterials() const override;
    // End UMeshComponent interface.

    void UpdateBodySetup();
    void UpdateCollision();

private:
    // Begin USceneComponent interface.
    virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
    // Begin USceneComponent interface.

    /** */
    TArray<FProceduralMeshTriangle> ProceduralMeshTris;

    friend class FProceduralMeshSceneProxy;
};