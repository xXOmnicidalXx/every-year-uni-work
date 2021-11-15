// Desert Racer.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <iostream>
#include <sstream>
#include <string.h>
#include <vector>

using namespace tle;
using namespace std;

// COLLISON FUNCTIONS CALLED FROM THE BOTTOM
bool PointToBoxCollisions(IModel* Box, float BoxWidth, float BoxLength, IModel* point);
bool SphereToBox(IModel* Box, float SphereRadius, IModel* Floor, float FloorWidth, float FloorHeight);
bool SpehereToSphere(IModel* A, float AR, IModel* B, float BR);

// CREATING VECTOR 2
struct Vector2
{
	float X;
	float Y;
};

// CREATING VECTOR 3
struct Vector3
{
	float X;
	float Y;
	float Z;
};

float GameSpeed = 0.0f;
float GameRotationSpeed = 100.0f;

// MOVEMENT FUNCTIONS
void Movement(I3DEngine* myEngine, IModel* HoverCar, ICamera* MyCamera)
{
	//const float kGameSpeed = 50.0f;
	float DeltaTime = myEngine->Timer();
	float CameraSpeed = 200.0f;

	// SLOWLY INCREASES SPEED/THRUST
	if (myEngine->KeyHeld(Key_W))
	{
		GameSpeed += 0.01f;
		if (GameSpeed >= 100.0f)
		{
			GameSpeed = 100.0f;
		}
	}
	// SLOWLY USES BACKTRUST TO REVERSE OR SLOW DOWN THE CAR
	else if (myEngine->KeyHeld(Key_S))
	{
		GameSpeed -= 0.01f;
		if (GameSpeed <= -100.0f)
		{
			GameSpeed = -100.0f;
		}
	}

	// PUSHES THE CAR FORWARD WITH FORCE
	HoverCar->MoveLocalZ(GameSpeed * DeltaTime);

	// ROTATES HOVERCAR ON THE YAXIS TO TURN CORNERS
	if (myEngine->KeyHeld(Key_A))
	{
		HoverCar->RotateY(-GameRotationSpeed * DeltaTime);
	}
	else if (myEngine->KeyHeld(Key_D))
	{
		HoverCar->RotateY(GameRotationSpeed * DeltaTime);
	}

	// CHANGES THE CAMERA LOCATION WHEN ARROW KEYS ARE PRESSED
	if (myEngine->KeyHeld(Key_Up))
	{
		MyCamera->MoveLocalZ(CameraSpeed * DeltaTime);
	}
	else if (myEngine->KeyHeld(Key_Down))
	{
		MyCamera->MoveLocalZ(-CameraSpeed * DeltaTime);
	}

	if (myEngine->KeyHeld(Key_Left))
	{
		MyCamera->MoveLocalX(-CameraSpeed * DeltaTime);
	}
	else if (myEngine->KeyHeld(Key_Right))
	{
		MyCamera->MoveLocalX(CameraSpeed * DeltaTime);
	}
}

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();
	//
	//
	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("S:\\TL-Engine\\Media");
	//
	// CAMERA SETUP AND SETTINGS
	ICamera* MyCamera;
	MyCamera = myEngine->CreateCamera(kManual);
	MyCamera->SetPosition(0.0f, 0.0f, 0.0f);
	MyCamera->RotateLocalX(20.0f);
	//
	//BUILDING THE BASIC LAYOUT
	IMesh* SkyboxMesh = myEngine->LoadMesh("skybox 07.x");
	IModel* Skybox = SkyboxMesh->CreateModel(0.0f, -960.0f, 0.0f);
	IMesh* GroundMesh = myEngine->LoadMesh("Ground.x");
	IModel* Ground = GroundMesh->CreateModel();
	//
	//BUILDING ARRAYS FOR RACE TRACK
	const int CheckpointArraySize = 4;
	IMesh* CheckpointMesh = myEngine->LoadMesh("checkpoint.x");
	IModel* CheckpointArray[CheckpointArraySize];
	CheckpointArray[0] = CheckpointMesh->CreateModel(0.0f, 0.0f, 0.0f);
	CheckpointArray[1] = CheckpointMesh->CreateModel(10.0f, 0.0f, 120.0f);
	CheckpointArray[1]->RotateY(90.0f);

	CheckpointArray[2] = CheckpointMesh->CreateModel(25.0f, 0.0f, 56.0f);
	CheckpointArray[3] = CheckpointMesh->CreateModel(10.0f, 0.0f, -15.0f);
	CheckpointArray[3]->RotateY(90.0f);

	const float CheckpointOffSet = 9.8f;
	//
	//
	const int DummyArraySize = CheckpointArraySize * 2;
	IMesh* DummyMesh = myEngine->LoadMesh("dummy.x");
	IModel* DummyArray[DummyArraySize];
	DummyArray[0] = DummyMesh->CreateModel(CheckpointArray[0]->GetX() - CheckpointOffSet, CheckpointArray[0]->GetY(), CheckpointArray[0]->GetZ());
	DummyArray[1] = DummyMesh->CreateModel(CheckpointArray[0]->GetX() + CheckpointOffSet, CheckpointArray[0]->GetY(), CheckpointArray[0]->GetZ());

	DummyArray[2] = DummyMesh->CreateModel(CheckpointArray[1]->GetX() , CheckpointArray[1]->GetY(), CheckpointArray[1]->GetZ() - CheckpointOffSet);
	DummyArray[3] = DummyMesh->CreateModel(CheckpointArray[1]->GetX() , CheckpointArray[1]->GetY(), CheckpointArray[1]->GetZ() + CheckpointOffSet);

	DummyArray[4] = DummyMesh->CreateModel(CheckpointArray[2]->GetX() - CheckpointOffSet, CheckpointArray[2]->GetY(), CheckpointArray[2]->GetZ());
	DummyArray[5] = DummyMesh->CreateModel(CheckpointArray[2]->GetX() + CheckpointOffSet, CheckpointArray[2]->GetY(), CheckpointArray[2]->GetZ());

	DummyArray[6] = DummyMesh->CreateModel(CheckpointArray[3]->GetX() , CheckpointArray[3]->GetY(), CheckpointArray[3]->GetZ() - CheckpointOffSet);
	DummyArray[7] = DummyMesh->CreateModel(CheckpointArray[3]->GetX() , CheckpointArray[3]->GetY(), CheckpointArray[3]->GetZ() + CheckpointOffSet);
	//
	//
	const int IsleArraySize = 17;
	IMesh* IsleMesh = myEngine->LoadMesh("isleStraight.x");
	IModel* IsleArray[IsleArraySize];
	IsleArray[0] = IsleMesh->CreateModel(-10.0f, 0.0f, 8.0f);
	IsleArray[1] = IsleMesh->CreateModel(10.0f, 0.0f, 8.0f);
	IsleArray[2] = IsleMesh->CreateModel(-10.0f, 0.0f, 24.0f);
	IsleArray[3] = IsleMesh->CreateModel(10.0f, 0.0f, 24.0f);
	IsleArray[4] = IsleMesh->CreateModel(-10.0f, 0.0f, 40.0f);
	IsleArray[5] = IsleMesh->CreateModel(10.0f, 0.0f, 40.0f);
	IsleArray[6] = IsleMesh->CreateModel(-10.0f, 0.0f, 56.0f);
	IsleArray[7] = IsleMesh->CreateModel(10.0f, 0.0f, 56.0f);
	IsleArray[8] = IsleMesh->CreateModel(-10.0f, 0.0f, 72.0f);
	IsleArray[9] = IsleMesh->CreateModel(10.0f, 0.0f, 72.0f);
	IsleArray[10] = IsleMesh->CreateModel(-10.0f, 0.0f, 88.0f);
	IsleArray[11] = IsleMesh->CreateModel(10.0f, 0.0f, 88.0f);
	IsleArray[12] = IsleMesh->CreateModel(-10.0f, 0.0f, 104.0f);
	IsleArray[13] = IsleMesh->CreateModel(10.0f, 0.0f, 104.0f);
	IsleArray[14] = IsleMesh->CreateModel(-10.0f, 0.0f, 120.0f);
	IsleArray[15] = IsleMesh->CreateModel(-10.0f, 0.0f, 136.0f);
	IsleArray[16] = IsleMesh->CreateModel(10.0f, 0.0f, 136.0f);
	//
	//
	const int WallArraySize = 20;
	IMesh* WallMesh = myEngine->LoadMesh("wall.x");
	IModel* WallArray[WallArraySize];
	WallArray[0] = WallMesh->CreateModel(-10.0f, 0.0f, 0.0f);
	WallArray[1] = WallMesh->CreateModel(10.0f, 0.0f, 0.0f);
	WallArray[2] = WallMesh->CreateModel(-10.0f, 0.0f, 16.0f);
	WallArray[3] = WallMesh->CreateModel(10.0f, 0.0f, 16.0f);
	WallArray[4] = WallMesh->CreateModel(-10.0f, 0.0f, 32.0f);
	WallArray[5] = WallMesh->CreateModel(10.0f, 0.0f, 32.0f);
	WallArray[6] = WallMesh->CreateModel(-10.0f, 0.0f, 48.0f);
	WallArray[7] = WallMesh->CreateModel(10.0f, 0.0f, 48.0f);
	WallArray[8] = WallMesh->CreateModel(-10.0f, 0.0f, 64.0f);
	WallArray[9] = WallMesh->CreateModel(10.0f, 0.0f, 64.0f);
	WallArray[10] = WallMesh->CreateModel(-10.0f, 0.0f, 80.0f);
	WallArray[11] = WallMesh->CreateModel(10.0f, 0.0f, 80.0f);
	WallArray[12] = WallMesh->CreateModel(-10.0f, 0.0f, 96.0f);
	WallArray[13] = WallMesh->CreateModel(10.0f, 0.0f, 96.0f);
	WallArray[14] = WallMesh->CreateModel(-10.0f, 0.0f, 112.0f);
	WallArray[15] = WallMesh->CreateModel(-10.0f, 0.0f, 128.0f);
	WallArray[16] = WallMesh->CreateModel(-4.0f, 0.0f, 136.0f);
	WallArray[16]->RotateY(90.0f);

	WallArray[17] = WallMesh->CreateModel(4.0f, 0.0f, 136.0f);
	WallArray[17]->RotateY(90.0f);

	WallArray[18] = WallMesh->CreateModel(15.0f, 0.0f, 136.0f);
	WallArray[18]->RotateY(90.0f);

	WallArray[19] = WallMesh->CreateModel(25.0f, 0.0f, 136.0f);
	WallArray[19]->RotateY(90.0f);

	const int WaterTankArraySize = 5;
	IMesh* WaterTankMesh = myEngine->LoadMesh("TankSmall1.x");
	IModel* WaterTankArray[WaterTankArraySize];
	WaterTankArray[0] = WaterTankMesh->CreateModel(-5.0f, 0.0f, 125.0f);
	WaterTankArray[1] = WaterTankMesh->CreateModel(2.0f, 0.0f, 130.0f);
	WaterTankArray[2] = WaterTankMesh->CreateModel(20.0f, 0.0f, 130.0f);
	WaterTankArray[3] = WaterTankMesh->CreateModel(27.0f, 0.0f, 125.0f);

	WaterTankArray[4] = WaterTankMesh->CreateModel(25.0f, -5.0f, 20.0f);
	WaterTankArray[4]->RotateX(-45);

	//WaterTankArray[5] = WaterTankMesh->CreateModel(0.0f, 0.0f, 0.0f);
	//WaterTankArray[6] = WaterTankMesh->CreateModel(0.0f, 0.0f, 0.0f);
	//WaterTankArray[7] = WaterTankMesh->CreateModel(0.0f, 0.0f, 0.0f);
	//WaterTankArray[8] = WaterTankMesh->CreateModel(0.0f, 0.0f, 0.0f);
	//WaterTankArray[9] = WaterTankMesh->CreateModel(0.0f, 0.0f, 0.0f);
	//
	// BUILD HOVER CAR AND SET PERAMETRES
	IMesh* HoverCarMesh = myEngine->LoadMesh("race2.x");
	IModel* HoverCar = HoverCarMesh->CreateModel(0.0f, 0.0f, -20.0f);
	//
	//
	MyCamera->AttachToParent(HoverCar);
	MyCamera->SetLocalPosition(0.0f, 30.0f, -50.0f);
	//
	// ADD BACKDROP FOR STYRINGS TO BE SHOWN
	ISprite* UI_BackdropSprite = myEngine->CreateSprite("ui_backdrop.jpg");
	//
	//
	stringstream GameStateString;
	IFont* GameStateFont = myEngine->LoadFont("Comic Sans MS", 36U);

	stringstream StartCountDownString;
	IFont* StartCountDownFont = myEngine->LoadFont("Comic Sans MS", 72U);
	float CountDown = 3.0f;

	stringstream CheckpointNotifictionString;
	IFont* CheckpointNotificationFont = myEngine->LoadFont("Comic Sans MS", 36U);

	stringstream HoverCarSpeedString;
	IFont* HoverCarSpeedFont = myEngine->LoadFont("Comic Sans MS", 36U);
	//
	// ENUM STATES
	enum EGameState { Playing, Paused, GameOver, Won };
	EGameState GameState = Paused;

	enum ECountDown { Three, Two, One, Go };
	ECountDown GameCountDown = Three;

	enum EcheckpointStage { Waiting_Checkpoint, Stage_1_Complete, Stage_2_Complete, Stage_3_Complete,Stage_4_Complete, Race_Complete };
	EcheckpointStage CheckpointState = Waiting_Checkpoint;

	enum ECameraAngle{DefaultChase, FirstPerson};
	ECameraAngle CameraAngle = DefaultChase;
	//
	//
	const Vector2 CheckpointDem = { 19.0f, 2.5f };
	const Vector2 MiddleWallRowDem = { 1.0f, 4.8f };
	const float kHoverCarRadius = 3.0f;
	const float WaterTankRadius = 4.5f;
	Vector2 OldHoverCarPosition;
	//
	// REMOVES MOUSE FROM FULL SCREEN
	myEngine->StartMouseCapture();
	//
	//
	float UpperLimit = 50.0f;
	float StandardCameraAngle = 20.0f;
	//
	//
	float CameraRotationX = 0.0f;
	float CameraRotationY = 0.0f;
	//
	//
	int HoverCarHealth = 100;
	float DelayTimer = 0.1f;

	bool HealthTimer = false;
	//
	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		float DeltaTime = myEngine->Timer();

		// Draw the scene (update scene)
		myEngine->DrawScene();
		
		
		// DIFFERENT STATES OF THE GAME
		switch (GameState)
		{
		case Playing:
			// SHOWS THE GAME STATE TOP LEFT OF THE SCREEN
			GameStateString << "Playing";
			GameStateFont->Draw(GameStateString.str(), 50.0f, 0.0f, kBlack, kCentre);
			GameStateString.str("");

			// COUNTDOWN FOR THE GAME TO START
			CountDown -= DeltaTime;

			if (CountDown <= 0.0f)
			{
				CountDown = 0.0f;
				StartCountDownString.str("");

				GameCountDown = Go;
			}
			else
			{
				StartCountDownString << (int)CountDown;
				StartCountDownFont->Draw(StartCountDownString.str(), myEngine->GetWidth() / 2, myEngine->GetHeight() / 2, kBlack, kCentre);
				StartCountDownString.str("");
			}
			//
			//
			switch (GameCountDown)
			{
			case Three:
				break;
			case Two:
				break;
			case One:
				break;
			case Go:
				// OLD CAR POSTION 
				OldHoverCarPosition.X = HoverCar->GetX();
				OldHoverCarPosition.Y = HoverCar->GetZ();

				// CALLED FUNCTION FOR THE MOVEMENT OF THE HOVER CAR
				Movement(myEngine, HoverCar, MyCamera);

				// BOOLS FOR THE FOR LOOPS AND COLLISIONS
				bool HoverCarToWall = false;
				bool HoverCarToStrut = false;
				bool HoverCarToWaterTank = false;
				bool Collision = false;
				
				// HOVER CAR TO WALL ARRAY COLLISION DETECTION
				for (int i = 0; i < WallArraySize; i++)
				{
					if (!HoverCarToWall)
					{
						HoverCarToWall = SphereToBox(HoverCar, kHoverCarRadius, WallArray[i], MiddleWallRowDem.X, MiddleWallRowDem.Y);
					}
				}
				//
				// HOVER CAR TO CHECKPOINT LEGS COLLISION DETECTION
				for (int i = 0; i < DummyArraySize; i++)
				{
					if (!HoverCarToStrut)
					{
						HoverCarToStrut = SpehereToSphere(HoverCar, kHoverCarRadius, DummyArray[i], 1);
					}
				}
				//
				// HOVER CAR TO WATER TANK COLLISION DETECTION
				for (int i = 0; i < WaterTankArraySize; i++)
				{
					if (!HoverCarToWaterTank)
					{
						HoverCarToWaterTank = SpehereToSphere(HoverCar, kHoverCarRadius, WaterTankArray[i], WaterTankRadius);
					}
				}
				//
				// IF HOVER CAR COLLIDES THEN APPLY OLD HOVER CAR LOCATION
				Collision = HoverCarToWall || HoverCarToStrut || HoverCarToWaterTank;
				if (Collision)
				{
					HoverCar->SetPosition(OldHoverCarPosition.X, 0.0f, OldHoverCarPosition.Y);
					
					if (!HealthTimer)
					{
						HoverCarHealth -= 1.0f;
						HealthTimer = true;
					}
				}
				// CAR HEALTH AND TIMER SO HEALTH DOESNT DEPLEATE INSTANTLY 
				if (HealthTimer)
				{
					DelayTimer -= DeltaTime;
					if (DelayTimer <= 0)
					{
						DelayTimer = 0.1;
						HealthTimer = false;
					}
				}
				//
				//

				// -6.4 <> 6.4 = 12/13 units (hover car)....hover car is 4 metres longs....12/4=3....3 is used to divide the UP/S to give speed (m/h)
				HoverCarSpeedString << "Speed MPH: " << (int)((GameSpeed * DeltaTime * (1 / DeltaTime)) / 3 *60 *60) /1610;
				HoverCarSpeedFont->Draw(HoverCarSpeedString.str(), 500.0f, 0.0f, kBlack, kCentre);
				HoverCarSpeedString.str("");

				GameStateString << "Health: " << (int)HoverCarHealth;
				GameStateFont->Draw(GameStateString.str(), 900.0f, 0.0f, kBlack, kCentre);
				GameStateString.str("");
				//
				//
				float MouseMovementX = myEngine->GetMouseMovementX();
				float MouseMovementY = myEngine->GetMouseMovementY();

				CameraRotationY += MouseMovementX;
				CameraRotationX += MouseMovementY;
				//
				// CAMERA ROTATION FIX
				if (CameraRotationX > 90.0f)
				{
					CameraRotationX -= MouseMovementY;
				}
				else if (CameraRotationX < -90.0f)
				{
					CameraRotationX -= MouseMovementY;
				}
				else
				{
					MyCamera->RotateLocalX(MouseMovementY);
				}
				//
				// CAMERA ROTATION FIX
				if (CameraRotationY > 90.0f)
				{
					CameraRotationY -= MouseMovementX;
				}
				else if (CameraRotationY < -90.0f)
				{
					CameraRotationY -= MouseMovementX;
				}
				else
				{
					MyCamera->RotateY(MouseMovementX);
				}
				//
				// DEFAULT VIEW
				if (myEngine->KeyHit(Key_1) && CameraAngle != DefaultChase)
				{
					if (CameraAngle == FirstPerson)
					{
						MyCamera->RotateX(0.0);
					}
					CameraAngle = DefaultChase;

					MyCamera->DetachFromParent();
					MyCamera->AttachToParent(HoverCar);
					MyCamera->SetLocalPosition(0.0f, 20.0f, -40.0f);
					MyCamera->RotateX(StandardCameraAngle);
				}
				//
				// FIRST PERSON VIEW
				if (myEngine->KeyHit(Key_2) && CameraAngle != FirstPerson)
				{

					if (CameraAngle == DefaultChase)
					{
						MyCamera->RotateX(-StandardCameraAngle);
					}
					CameraAngle = FirstPerson;

					MyCamera->DetachFromParent();
					MyCamera->AttachToParent(HoverCar);
					MyCamera->SetLocalPosition(0.0f, 5.0f, 1.0f);
					MyCamera->RotateX(0.0f);
				}
				//
				//
				if (CheckpointState == Race_Complete)
				{
					GameState = Won;
				}
				//
				//
				if (HoverCarHealth == 0)
				{
					GameState = GameOver;
				}

				break;
			}
			//
			//
			break;
			// GAME STATES ON THE TOP LEFT WHEN THE STATES HAVE CHANGED
		case Paused:
			GameStateString << "Paused";
			GameStateFont->Draw(GameStateString.str(), 50.0f, 0.0f, kBlack, kCentre);
			GameStateString.str("");

			GameStateString << "Hit Space to Start";
			GameStateFont->Draw(GameStateString.str(), myEngine->GetWidth() / 2, myEngine->GetHeight() / 2, kRed, kCentre);
			GameStateString.str("");
			break;

		case GameOver:
			GameStateString << "GameOver";
			GameStateFont->Draw(GameStateString.str(), 50.0f, 0.0f, kBlack, kCentre);
			GameStateString.str("");
			break;

		case Won:
			GameStateString << "Race Won";
			GameStateFont->Draw(GameStateString.str(), 50.0f, 0.0f, kBlack, kCentre);
			GameStateString.str("");

			break;
		}

		// CHECKPOINT DEMENSIONS FOR CHECKING IF THE CAR HAS GONE THROUGH THE CHECK POINT IN THE CORRECT ORDER
		bool HoverCarCheckpoint_1 = PointToBoxCollisions(CheckpointArray[0], CheckpointDem.X, CheckpointDem.Y, HoverCar);
		bool HoverCarCheckpoint_2 = PointToBoxCollisions(CheckpointArray[1], CheckpointDem.X, CheckpointDem.Y, HoverCar);
		bool HoverCarCheckpoint_3 = PointToBoxCollisions(CheckpointArray[2], CheckpointDem.X, CheckpointDem.Y, HoverCar);
		bool HoverCarCheckpoint_4 = PointToBoxCollisions(CheckpointArray[3], CheckpointDem.X, CheckpointDem.Y, HoverCar);
		bool HoverCarCheckpoint_5 = PointToBoxCollisions(CheckpointArray[0], CheckpointDem.X, CheckpointDem.Y, HoverCar);

		switch (CheckpointState)
		{
		case Waiting_Checkpoint:

			CheckpointNotifictionString << "Awaiting Checkpoint";
			CheckpointNotificationFont->Draw(CheckpointNotifictionString.str(), 250.0f, 0.0f, kBlack, kCentre);
			CheckpointNotifictionString.str("");

			if (HoverCarCheckpoint_1)
			{
				CheckpointState = Stage_1_Complete;
			}

			break;

		case Stage_1_Complete:

			CheckpointNotifictionString << "Stage 1 Complete";
			CheckpointNotificationFont->Draw(CheckpointNotifictionString.str(), 250.0f, 0.0f, kBlack, kCentre);
			CheckpointNotifictionString.str("");

			if (HoverCarCheckpoint_2)
			{
				CheckpointState = Stage_2_Complete;
			}

			break;

		case Stage_2_Complete:
			CheckpointNotifictionString << "Stage 2 Complete";
			CheckpointNotificationFont->Draw(CheckpointNotifictionString.str(), 250.0f, 0.0f, kBlack, kCentre);
			CheckpointNotifictionString.str("");

			if (HoverCarCheckpoint_3)
			{
				CheckpointState = Stage_3_Complete;
			}

			break;

		case Stage_3_Complete:
			CheckpointNotifictionString << "Stage 3 Complete";
			CheckpointNotificationFont->Draw(CheckpointNotifictionString.str(), 250.0f, 0.0f, kBlack, kCentre);
			CheckpointNotifictionString.str("");

			if (HoverCarCheckpoint_4)
			{
				CheckpointState = Stage_4_Complete;
			}

			break;

		case Stage_4_Complete:
			CheckpointNotifictionString << "Stage 4 Complete";
			CheckpointNotificationFont->Draw(CheckpointNotifictionString.str(), 250.0f, 0.0f, kBlack, kCentre);
			CheckpointNotifictionString.str("");

			if (HoverCarCheckpoint_5)
			{
				CheckpointState = Race_Complete;
			}
			break;

		case Race_Complete:
			CheckpointNotifictionString << "Race Complete";
			CheckpointNotificationFont->Draw(CheckpointNotifictionString.str(), myEngine->GetWidth() / 2, myEngine->GetHeight() / 2, kBlack, kCentre);
			CheckpointNotifictionString.str("");
			break;
		}

		if (GameState == Paused)
		{
			if (myEngine->KeyHit(Key_Space))
			{
				GameState = Playing;
			}
		}

		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}

// COLLSIONS
bool PointToBoxCollisions(IModel* Box, float BoxWidth, float BoxLength, IModel* point)
{
	float CombinedWidth = (BoxWidth * 0.5f);
	float CombinedHeight = (BoxLength * 0.5f);

	if (point->GetX() >= Box->GetX() - CombinedWidth && point->GetX() <= Box->GetX() + CombinedWidth &&
		point->GetZ() >= Box->GetZ() - CombinedHeight && point->GetZ() <= Box->GetZ() + CombinedHeight)
	{
		return true;
	}
	return false;
}

bool SphereToBox(IModel* Box, float SphereRadius, IModel* Floor, float FloorWidth, float FloorHeight)
{
	float CombinedWidth = SphereRadius + FloorWidth * 0.5f;
	float CombinedHeight = SphereRadius + FloorHeight * 0.5f;

	if (Box->GetX() >= Floor->GetX() - CombinedWidth && Box->GetX() <= Floor->GetX() + CombinedWidth &&
		Box->GetZ() >= Floor->GetZ() - CombinedHeight && Box->GetZ() <= Floor->GetZ() + CombinedHeight)
	{
		return true;
	}
	return false;
}

bool SpehereToSphere(IModel* A, float AR, IModel* B, float BR)
{
	float	X, Y, Z;
	X = A->GetX() - B->GetX();
	Y = A->GetY() - B->GetY();
	Z = A->GetZ() - B->GetZ();

	float Distance = sqrt(X * X + Y * Y + Z * Z);

	if (Distance < AR + BR)
	{
		return true;
	}
	return false;
}