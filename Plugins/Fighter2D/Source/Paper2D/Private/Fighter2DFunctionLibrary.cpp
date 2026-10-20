// Fill out your copyright notice in the Description page of Project Settings.


#include "Fighter2DFunctionLibrary.h"

#include "PaperFlipbook.h"
#include "PaperSprite.h"
TArray<FSpriteShapeBP> UFighter2DFunctionLibrary::GetShapeListFromSprite(UPaperSprite* Sprite,
    TEnumAsByte<EShapeQuerySettings> Type)
{
    if(!Sprite)
    {
        UE_LOG(LogTemp,Error,TEXT("Sprite Object Was Not Valid"));
    }
    TArray<FSpriteShapeBP> out;


    for (FSpriteGeometryShape Shape: Sprite->CollisionGeometry.Shapes)
    {
       
        if(Type==EShapeQuerySettings::AllBoxes)
        {
            FSpriteShapeBP NewShape;
            NewShape.Owner=Sprite;
            NewShape.Shape=Shape;
            out.Add(NewShape);
    
        }
        //
        if(Type==EShapeQuerySettings::CustomBoxes)
        {
            if(Shape.ShapeRole==4)
            {
                FSpriteShapeBP NewShape;
                NewShape.Owner=Sprite;
                NewShape.Shape=Shape;
                out.Add(NewShape);
            }
        }
        //
        if(Type==EShapeQuerySettings::GuardBoxes)
        {
            if(Shape.ShapeRole==2)
            {
                FSpriteShapeBP NewShape;
                NewShape.Owner=Sprite;
                NewShape.Shape=Shape;
                out.Add(NewShape);
            }
        }
        //
        if(Type==EShapeQuerySettings::HitBoxes)
        {
            if(Shape.ShapeRole==1)
            {
                FSpriteShapeBP NewShape;
                NewShape.Owner=Sprite;
                NewShape.Shape=Shape;
                out.Add(NewShape);
            }
        }
        //
        if(Type==EShapeQuerySettings::TraceBoxes)
        {
            if(Shape.ShapeRole==3)
            {
                FSpriteShapeBP NewShape;
                NewShape.Owner=Sprite;
                NewShape.Shape=Shape;
                out.Add(NewShape);
            }
        
        }
    }
    return out;
}

TEnumAsByte<EShapeRoleType> UFighter2DFunctionLibrary::GetShapeType(FSpriteShapeBP InShape)
{
  
    
  return  static_cast<EShapeRoleType>( InShape.Shape.ShapeRole);
}

FVector2D UFighter2DFunctionLibrary::GetBoxSize(FSpriteShapeBP InShape)
{
    
    if(InShape.Shape.ShapeType!=ESpriteShapeType::Box)
    {
        UE_LOG(LogTemp,Error,TEXT("Object IsNot A Box"));
    }
    return InShape.Shape.BoxSize;
}

FVector2D UFighter2DFunctionLibrary::GetBoxPosition( FSpriteShapeBP InShape)
{
    if(!InShape.Owner)
    {
        UE_LOG(LogTemp,Error,TEXT("OwnerSprite not Valid"));
    }
#if WITH_EDITORONLY_DATA
    return  (InShape.Shape.BoxPosition- InShape.Owner->GetPivotPosition())*FVector2D(1,-1);
#else
    return  (InShape.Shape.BoxPosition- InShape.Owner->BakedPivotPosition)*FVector2D(1,-1);
#endif
    
}


TArray<FVector2D> UFighter2DFunctionLibrary::GetVerticesFromShape(FSpriteShapeBP InShape)
{
    return  InShape.Shape.Vertices;
    
}

void UFighter2DFunctionLibrary::SetTextureOverride(UPaperFlipbook* PaperFlipbook, int Input)
{
    if(PaperFlipbook)
    {
        const int NumFrames=PaperFlipbook->GetNumFrames();
     for (int Index=0;Index!=  NumFrames ;++Index){
         
      auto Sprite=   PaperFlipbook->GetSpriteAtFrame(Index);
         
         if(Sprite)
         {
             Sprite->SetTextureOverride(Input);
         }

     }
    }
}

void UFighter2DFunctionLibrary::GetAllSprites(TArray<UPaperSprite*>& PaperSprites, UPaperFlipbook* Flipbook)
{

    const int NumFrames=Flipbook->GetNumFrames();
    for (int Index=0;Index!=  NumFrames ;++Index){
         
        auto Sprite=   Flipbook->GetSpriteAtFrame(Index);
         
      PaperSprites.Add(Sprite);

    }
}
