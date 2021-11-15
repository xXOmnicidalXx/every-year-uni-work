// future space frogs.cpp: A program using the TL-Engine

#include <TL-Engine.h>// TL-Engine include file and namespace
#include <iostream>
#include <sstream>
#include <string.h>

using namespace tle;
using namespace std;

bool SpehereToSphere(IModel* A, float AR, IModel* B, float BR);
bool BoxToSphere(IModel* Box, float BoxWidth, float BoxHeight, IModel* Sphere, float SpehereRadius);
bool BoxToBox(IModel* Car, float CarWidth, float CarHeight, IModel* Floor, float FloorWidth, float FloorHeight);
bool SphereToBox(IModel* Frog, float SphereRadius, IModel* Floor, float FloorWidth, float FloorHeight);

struct Vector2
{
	float X;
	float Y;
};

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("S:\\TL-Engine\\Media");

	/**** Set up your scene here ****/

	// CAMERA SETUP & POSITION
	ICamera* MyCamera;
	MyCamera = myEngine->CreateCamera(kManual);
	MyCamera->SetPosition(0.0f, 120.0f, -100.0f);
	MyCamera->RotateX(50.0f);

	// SKY SETUP & POSITION
	IMesh* SkyboxMesh = myEngine->LoadMesh("skybox.x");
	IModel* Skybox = SkyboxMesh->CreateModel(0.0f, -960.0f, 0.0f);

	// FLOOR SETUP & POSITION
	IMesh* FloorMesh = myEngine->LoadMesh("Surface.x");
	IModel* Floor = FloorMesh->CreateModel(0.0f, -5.0f, 0.0f);

	// ISLAND SETUP & POSITION
	IMesh* IslandMesh = myEngine->LoadMesh("island.x");
	IModel* Island = IslandMesh->CreateModel(0.0f, -6.0f, 0.0f);

	// CAR SETUP & POSITION
	IMesh* CarMesh = myEngine->LoadMesh("rover.x");
	IModel* Car = CarMesh->CreateModel(0.0f, 0.0f, -50.0f);

	// FROGS SETUP & POSITIONS
	IMesh* FrogMesh = myEngine->LoadMesh("frog.x");
	IModel* Frog_1 = FrogMesh->CreateModel(5.0f, 0.0f, 0.0f);
	IModel* Frog_2 = FrogMesh->CreateModel(-5.0f, 0.0f, 0.0f);
	IModel* Frog_3 = FrogMesh->CreateModel(0.0f, 0.0f, -5.0f);
	IModel* Frog_4 = FrogMesh->CreateModel(0.0f, 0.0f, 5.0f);
	//IModel* Frog_5 = FrogMesh->CreateModel(0.0f, 20.0f, 0.0f); // (test frog)
	Frog_1->RotateY(90.0f);
	Frog_2->RotateY(-90.0f);
	Frog_3->RotateY(180.0f);

	// PLANTS SETUP & POSITIONS
	IMesh* PlantMesh = myEngine->LoadMesh("plant.x");
	IModel* Plant_1 = PlantMesh->CreateModel(50.0f, 0.0f, 25.0f);
	IModel* Plant_2 = PlantMesh->CreateModel(-50.0f, 0.0f, -25.0f);
	IModel* Plant_3 = PlantMesh->CreateModel(25.0f, 0.0f, 50.0f);
	IModel* Plant_4 = PlantMesh->CreateModel(-25.0f, 0.0f, -50.0f);
	IModel* Plant_5 = PlantMesh->CreateModel(50.0f, 0.0f, -50.0f);
	IModel* Plant_6 = PlantMesh->CreateModel(-50.0f, 0.0f, 50.0f);

	// PORTAL SETUP AND POSITION
	IMesh* PortalMesh = myEngine->LoadMesh("portal.x");
	IModel* Portal = PortalMesh->CreateModel();
	Portal->Scale(1.2);

	// GAME STATE
	enum EGame_State { Playing, Paused, Over, Won };
	EGame_State Game_State = Playing;

	// FROG STATE
	enum EFrog_State { Waiting, Moving, Dead, Escaped };
	EFrog_State frog_1_State = Moving;
	EFrog_State frog_2_State = Waiting;
	EFrog_State frog_3_State = Waiting;
	EFrog_State frog_4_State = Waiting;

	// WATER STATE
	enum EWaterState { Safe, Poisoned };
	EWaterState Water_State = Safe;

	// CAMERA ANGLE STATE
	enum ECarAngle { Default, Chase, FirstPerson };
	ECarAngle CarAngle = Default;

	enum eFrogHeight {Up, Down};
	eFrogHeight FrogHeight_1 = Up;
	eFrogHeight FrogHeight_2 = Up;
	eFrogHeight FrogHeight_3 = Up;
	eFrogHeight FrogHeight_4 = Up;

	const float kGameSpeed = 100.0f; // CAR SPEED
	float Frog_Speed = kGameSpeed / 20; // FROG SPEED

	const float kGameResume = kGameSpeed;

	const float kGamePause = 0.0f;
	bool PauseGame = false;

	const float kGameOver = 0.0f;
	bool GameOver = false;

	float Frog_Z_Rotation = 180.0f;

	const float kFrog_Hop_Height = 5.0f;
	const float	kFloor = 0.0f;

	// TEXT SETUP FOR SCORE
	float Score = 0.0f;
	stringstream Score_String;
	IFont* Score_Font = myEngine->LoadFont("Comic Sans MS", 36U);

	// TEXT SETUP FOR GAME STATE
	stringstream GameState_String;
	IFont* GameState_Font = myEngine->LoadFont("Comic Sans MS", 72U);

	// TEXT SETUP FOR TIMER
	stringstream Timer_String;
	IFont* Timer_Font = myEngine->LoadFont("Comic Sans MS", 36U);

	const float kFrogRadius = 8.0f; // FROGS
	const float kCarRadius = 2.0f; // CAR (SPHERE COLLISION)
	const Vector2 CarDem = { 2.79333f, 5.9069f }; // CAR (SQUARE COLLISON)
	const Vector2 FloorDem = { 125.0f, 125.0f }; // FLOOR
	const Vector2 PortalDem = { 28.0f, 28.0f }; //PORTAL
	const float kPlantRadius = 1.5f; // PLANT

	// POSTION THE CAR WAS JUST AT (X,Z)
	Vector2 OldCarPosition;

	// POSITION THE FROG WAS JUST AT (X,Z)
	Vector2 OldFrogPosition;

	// TIMER FOR 1 MINUTE
	float MinuteTimer = 60.0f;

	// TIMER FOR 1 SECOND
	float OneSecondTimer = 1.0f;

	// HEIGHT THE FROG CAN JUMP
	const float KFrogHeight = 5.0f;


	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		float DeltaTime = myEngine->Timer();

		// Draw the scene (Update your scene each frame here)
		myEngine->DrawScene();

		// SCORE TOP LEFT CORNER
		Score_String << "Score: " << Score;
		Score_Font->Draw(Score_String.str(), 0.0f, 0.0f, kBlack);
		Score_String.str("");

		Timer_String << "Timer: " << (int)MinuteTimer;
		Timer_Font->Draw(Timer_String.str(), myEngine->GetWidth() / 2, 0.0f, kBlack, kCentre);
		Timer_String.str("");

		// WHAT HAPPENS WHEN GAME OVER
		if (Game_State == Over)
		{
			if (GameOver == false)
			{
				GameOver = true;
			}

			else if (GameOver == true)
			{
				Car->MoveX(kGameOver);
				Car->RotateZ(kGameOver);
				Frog_1->MoveX(kGameOver);
				Frog_2->MoveX(kGameOver);
				Frog_3->MoveZ(kGameOver);
				Frog_4->MoveZ(kGameOver);

				GameOver = false;
			}
		}

		// PAUSES GAME AND STOPS ANYTHING
		if (myEngine->KeyHit(Key_P))
		{
			if (PauseGame == false)
			{
				PauseGame = true;
				Game_State = Paused;
			}
			else
			{
				PauseGame = false;
				Game_State = Playing;
			}
		}

		//GAME STATE SWITCH (SWITCHING STATES OF PLAY)
		switch (Game_State)
		{
		case Playing:
			break;

		case Paused:
			GameState_String << "PAUSED";
			GameState_Font->Draw(GameState_String.str(), myEngine->GetWidth() / 2, myEngine->GetHeight() / 2, kRed, kCentre);
			GameState_String.str("");
			break;

		case Over:
			GameState_String << "GAME OVER";
			GameState_Font->Draw(GameState_String.str(), myEngine->GetWidth() / 2, myEngine->GetHeight() / 2, kRed, kCentre);
			GameState_String.str("");
			break;

		case Won:
			GameState_String << "GAME WON";
			GameState_Font->Draw(GameState_String.str(), myEngine->GetWidth() / 2, myEngine->GetHeight() / 2, kRed, kCentre);
			GameState_String.str("");
			break;
		}

		// START OF THE GAME)
		if (GameOver == false)
		{
			// RESET THE GAME
			if (myEngine->KeyHit(Key_Space))
			{
				// RESET GAME MODE/STATE
				if (Game_State == Over)
				{
					Game_State = Playing;
				}

				if (Game_State == Won)
				{
					Game_State = Playing;
				}

				// RESET TIMER
				if (MinuteTimer <= 60)
				{
					MinuteTimer = 60;
				}

				// RESET SCORE
				Score = 0.0f;

				// RESET CAR POSITION
				Car->SetPosition(0.0f, 0.0f, -50.0f);
				Car->ResetOrientation();

				// RESET FROG STATE
				frog_1_State = Moving;
				frog_2_State = Waiting;
				frog_3_State = Waiting;
				frog_4_State = Waiting;

				// RESET FROG POSITIONS
				Frog_1->SetPosition(5.0f, 0.0f, 0.0f);
				Frog_1->ResetOrientation();
				Frog_1->RotateY(90.0f);
				Frog_1->ResetScale();

				Frog_2->SetPosition(-5.0f, 0.0f, 0.0f);
				Frog_2->ResetOrientation();
				Frog_2->RotateY(-90.0f);
				Frog_2->ResetScale();

				Frog_3->SetPosition(0.0f, 0.0f, -5.0f);
				Frog_3->ResetOrientation();
				Frog_3->RotateY(180.0f);
				Frog_3->ResetScale();

				Frog_4->SetPosition(0.0f, 0.0f, 5.0f);
				Frog_4->ResetOrientation();
				Frog_4->ResetScale();

				// WATER STATE
				Water_State = Safe;
				Floor->SetSkin("water.jpg");

				// RESET CAMERA VIEW
				if (CarAngle != Default)
				{
					if (CarAngle == Chase)
					{
						MyCamera->RotateX(-15.0f);
					}

					CarAngle = Default;

					MyCamera->DetachFromParent();
					MyCamera->SetPosition(0.0f, 120.0f, -100.0f);
					MyCamera->RotateX(50.0f);
				}
			}

			// GAME TIMER
			if (MinuteTimer <= 0.0f)
			{
				MinuteTimer = 0.0f;

				// CHECKING STATES OF FROGS & GAME STATE
				if (frog_1_State, frog_2_State, frog_3_State, frog_4_State == Waiting || frog_1_State, frog_2_State, frog_3_State, frog_4_State == Dead)
				{
					if (frog_1_State == Moving)
					{
						if (frog_1_State == Escaped)
						{
							Game_State = Over;
						}
					}

					else
					{
						Game_State = Won;
					}

					if (frog_2_State == Moving)
					{
						if (frog_2_State == Escaped)
						{
							Game_State = Over;
						}
					}

					else
					{
						Game_State = Won;
					}

					if (frog_3_State == Moving)
					{
						if (frog_3_State == Escaped)
						{
							Game_State = Over;
						}
					}

					else
					{
						Game_State = Won;
					}

					if (frog_4_State == Moving)
					{
						if (frog_4_State == Escaped)
						{
							Game_State = Over;
						}
					}

					else
					{
						Game_State = Won;
					}
				}
			}

			if (PauseGame == false)
			{
				if (Game_State == Playing)
				{
					MinuteTimer -= DeltaTime; // COUNTDOWN

					OldCarPosition.X = Car->GetX();
					OldCarPosition.Y = Car->GetZ();

					// CAR MOVEMENT FORWARD & BACKWARD
					if (myEngine->KeyHeld(Key_W))
					{
						Car->MoveLocalZ(kGameSpeed * DeltaTime);
					}
					else if (myEngine->KeyHeld(Key_S))
					{
						Car->MoveLocalZ(-kGameSpeed * DeltaTime);
					}

					// CAR ROTATION CLOCKWISE & ANTI-CLOCKWISE
					if (myEngine->KeyHeld(Key_A))
					{
						Car->RotateY(-kGameSpeed * DeltaTime);
					}
					else if (myEngine->KeyHeld(Key_D))
					{
						Car->RotateY(kGameSpeed * DeltaTime);
					}

					// CAR TO PLANT COLLISION CHECK
					bool CarIntoPlant_1 = SpehereToSphere(Car, kFrogRadius, Plant_1, kPlantRadius);
					bool CarIntoPlant_2 = SpehereToSphere(Car, kFrogRadius, Plant_2, kPlantRadius);
					bool CarIntoPlant_3 = SpehereToSphere(Car, kFrogRadius, Plant_3, kPlantRadius);
					bool CarIntoPlant_4 = SpehereToSphere(Car, kFrogRadius, Plant_4, kPlantRadius);
					bool CarIntoPlant_5 = SpehereToSphere(Car, kFrogRadius, Plant_5, kPlantRadius);
					bool CarIntoPlant_6 = SpehereToSphere(Car, kFrogRadius, Plant_6, kPlantRadius);

					if (CarIntoPlant_1 || CarIntoPlant_2 || CarIntoPlant_3 || CarIntoPlant_4 || CarIntoPlant_5 || CarIntoPlant_6)
					{
						Car->SetPosition(OldCarPosition.X, 0.0f, OldCarPosition.Y);
					}

					// CAR & FLOOR COLLISION CHECK
					bool CarOffIsland = !BoxToBox(Car, CarDem.X, CarDem.Y, Floor, FloorDem.X, FloorDem.Y);

					if (CarOffIsland)
					{
						Game_State = Over;
					}

					// CAR TO PORTAL COLLISON CHECK
					bool CarIntoPortal = BoxToBox(Car, CarDem.X, CarDem.Y, Portal, PortalDem.X, PortalDem.Y);

					if (CarIntoPortal)
					{
						Car->SetPosition(OldCarPosition.X, 0.0f, OldCarPosition.Y);
					}

					// CAR & FROG COLLISION CHECK
					bool CollisionOccured_1 = BoxToSphere(Car, CarDem.X, CarDem.Y, Frog_1, kFrogRadius);
					bool CollisionOccured_2 = BoxToSphere(Car, CarDem.X, CarDem.Y, Frog_2, kFrogRadius);
					bool CollisionOccured_3 = BoxToSphere(Car, CarDem.X, CarDem.Y, Frog_3, kFrogRadius);
					bool CollisionOccured_4 = BoxToSphere(Car, CarDem.X, CarDem.Y, Frog_4, kFrogRadius);

					// FROG OFF THE FLOOR COLLISION
					bool Escaped_1 = SphereToBox(Frog_1, kFrogRadius, Floor, FloorDem.X, FloorDem.Y);
					bool Escaped_2 = SphereToBox(Frog_2, kFrogRadius, Floor, FloorDem.X, FloorDem.Y);
					bool Escaped_3 = SphereToBox(Frog_3, kFrogRadius, Floor, FloorDem.X, FloorDem.Y);
					bool Escaped_4 = SphereToBox(Frog_4, kFrogRadius, Floor, FloorDem.X, FloorDem.Y);

					// frog_1 state and collision
					if (frog_1_State == Moving)
					{
						Frog_1->MoveX(Frog_Speed * DeltaTime);
						//Frog_1->MoveY(kFrog_Hop_Height * DeltaTime);

						if (FrogHeight_1 == Up)
						{
							Frog_1->MoveY(kFrog_Hop_Height * DeltaTime);
							if (Frog_1->GetY() >= KFrogHeight)
							{
								FrogHeight_1 = Down;
							}
						}

						if (FrogHeight_1 == Down)
						{
							Frog_1->MoveY(-kFrog_Hop_Height * DeltaTime);
							if (Frog_1->GetY() <= kFloor)
							{
								FrogHeight_1 = Up;
							}
						}

						OneSecondTimer = 1.0f;

						if (CollisionOccured_1)
						{
							Frog_1->RotateZ(Frog_Z_Rotation);
							Frog_1->MoveY(0.50f);
							Frog_1->ScaleY(0.20f);

							Score = Score + 10.0f;

							frog_1_State = Dead;
						}
						else if (Escaped_1)
						{
							frog_1_State = Escaped;
							Floor->SetSkin("poisoned.png");
							Game_State = Over;
						}
					}

					if (frog_1_State == Dead)
					{
						OneSecondTimer -= DeltaTime;

						if (OneSecondTimer <= 0.0f)
						{
							frog_1_State = Waiting;

							Frog_1->RotateZ(-Frog_Z_Rotation);
							Frog_1->MoveY(-0.50f);
							Frog_1->ResetScale();
							Frog_1->SetPosition(5.0f, 0.0f, 0.0f);

							frog_2_State = Moving;
						}
					}

					// frog_2 state and collision
					if (frog_1_State == Waiting)
					{
						if (frog_2_State == Moving)
						{
							Frog_2->MoveX(-Frog_Speed * DeltaTime);

							if (FrogHeight_2 == Up)
							{
								Frog_2->MoveY(kFrog_Hop_Height * DeltaTime);
								if (Frog_2->GetY() >= KFrogHeight)
								{
									FrogHeight_2 = Down;
								}
							}

							if (FrogHeight_2 == Down)
							{
								Frog_2->MoveY(-kFrog_Hop_Height * DeltaTime);
								if (Frog_2->GetY() <= kFloor)
								{
									FrogHeight_2 = Up;
								}
							}

							OneSecondTimer = 1.0f;

							if (CollisionOccured_2)
							{
								Frog_2->RotateZ(Frog_Z_Rotation);
								Frog_2->MoveY(0.50f);
								Frog_2->ScaleY(0.20f);

								Score = Score + 10.0f;

								frog_2_State = Dead;
							}
							else if (Escaped_2)
							{
								frog_2_State = Escaped;
								Floor->SetSkin("poisoned.png");
								Game_State = Over;
							}
						}
					}

					if (frog_2_State == Dead)
					{
						OneSecondTimer -= DeltaTime;

						if (OneSecondTimer <= 0.0f)
						{
							frog_2_State = Waiting;

							Frog_2->RotateZ(-Frog_Z_Rotation);
							Frog_2->MoveY(-0.50f);
							Frog_2->ResetScale();
							Frog_2->SetPosition(-5.0f, 0.0f, 0.0f);

							frog_3_State = Moving;
						}
					}

					// frog_3 state and collision
					if (frog_2_State == Waiting)
					{
						if (frog_3_State == Moving)
						{
							Frog_3->MoveZ(-Frog_Speed * DeltaTime);

							if (FrogHeight_3 == Up)
							{
								Frog_3->MoveY(kFrog_Hop_Height * DeltaTime);
								if (Frog_3->GetY() >= KFrogHeight)
								{
									FrogHeight_3 = Down;
								}
							}

							if (FrogHeight_3 == Down)
							{
								Frog_3->MoveY(-kFrog_Hop_Height * DeltaTime);
								if (Frog_3->GetY() <= kFloor)
								{
									FrogHeight_3 = Up;
								}
							}

							OneSecondTimer = 1.0f;

							if (CollisionOccured_3)
							{
								Frog_3->RotateZ(Frog_Z_Rotation);
								Frog_3->MoveY(0.50f);
								Frog_3->ScaleY(0.20f);

								Score = Score + 10.0f;

								frog_3_State = Dead;
							}
							else if (Escaped_3)
							{
								frog_3_State = Escaped;
								Floor->SetSkin("poisoned.png");
								Game_State = Over;
							}
						}
					}

					if (frog_3_State == Dead)
					{
						OneSecondTimer -= DeltaTime;

						if (OneSecondTimer <= 0.0f)
						{
							frog_3_State = Waiting;

							Frog_3->RotateZ(-Frog_Z_Rotation);
							Frog_3->MoveY(-0.50f);
							Frog_3->ResetScale();
							Frog_3->SetPosition(0.0f, 0.0f, -5.0f);

							frog_4_State = Moving;
						}
					}

					// frog_4 state and collision
					if (frog_3_State == Waiting)
					{
						if (frog_4_State == Moving)
						{
							Frog_4->MoveZ(Frog_Speed * DeltaTime);

							if (FrogHeight_4 == Up)
							{
								Frog_4->MoveY(kFrog_Hop_Height * DeltaTime);
								if (Frog_4->GetY() >= KFrogHeight)
								{
									FrogHeight_4 = Down;
								}
							}

							if (FrogHeight_4 == Down)
							{
								Frog_4->MoveY(-kFrog_Hop_Height * DeltaTime);
								if (Frog_4->GetY() <= kFloor)
								{
									FrogHeight_4 = Up;
								}
							}

							OneSecondTimer = 1.0f;

							if (CollisionOccured_4)
							{
								Frog_4->RotateZ(Frog_Z_Rotation);
								Frog_4->MoveY(0.50f);
								Frog_4->ScaleY(0.20f);

								Score = Score + 10.0f;

								frog_4_State = Dead;
							}
							else if (Escaped_4)
							{
								frog_4_State = Escaped;
								Floor->SetSkin("poisoned.png");
								Game_State = Over;
							}
						}
					}

					if (frog_4_State == Dead)
					{
						OneSecondTimer -= DeltaTime;

						if (OneSecondTimer <= 0.0f)
						{
							frog_4_State = Waiting;

							Frog_4->RotateZ(-Frog_Z_Rotation);
							Frog_4->MoveY(-0.50f);
							Frog_4->ResetScale();
							Frog_4->SetPosition(0.0f, 0.0f, 5.0f);

							frog_1_State = Moving;
						}
					}

					// DEFAULT VIEW
					if (myEngine->KeyHit(Key_1) && CarAngle != Default)
					{
						if (CarAngle == Chase)
						{
							MyCamera->RotateX(-15.0f);
						}

						CarAngle = Default;

						MyCamera->DetachFromParent();
						MyCamera->SetPosition(0.0f, 120.0f, -100.0f);
						MyCamera->RotateX(50.0f);
					}

					// CHASE VIEW
					if (myEngine->KeyHit(Key_2) && CarAngle != Chase)
					{
						if (CarAngle == Default)
						{
							MyCamera->RotateX(-50.0f);
						}

						CarAngle = Chase;

						MyCamera->DetachFromParent();
						MyCamera->AttachToParent(Car);
						MyCamera->SetLocalPosition(0.0f, 30.0f, -50.0f);
						MyCamera->RotateX(15.0f);
					}

					// FIRST PERSON VIEW
					if (myEngine->KeyHit(Key_3) && CarAngle != FirstPerson)
					{
						if (CarAngle == Chase)
						{
							MyCamera->RotateX(-15.0f);
						}
						else if (CarAngle == Default)
						{
							MyCamera->RotateX(-50.0f);
						}

						CarAngle = FirstPerson;

						MyCamera->DetachFromParent();
						MyCamera->AttachToParent(Car);
						MyCamera->SetLocalPosition(0.0f, 4.5f, -2.0f);
					}
				}
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




// CAR TO PLANT
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

// CAR TO FROG
bool BoxToSphere(IModel* Box, float BoxWidth, float BoxHeight, IModel* Sphere, float SpehereRadius)
{
	float CombinedWidth = (BoxWidth * 0.5f) + SpehereRadius;
	float CombinedHeight = (BoxHeight * 0.5f) + SpehereRadius;

	if (Sphere->GetX() >= Box->GetX() - CombinedWidth && Sphere->GetX() <= Box->GetX() + CombinedWidth &&
		Sphere->GetZ() >= Box->GetZ() - CombinedHeight && Sphere->GetZ() <= Box->GetZ() + CombinedHeight)
	{
		return true;
	}
	return false;
}

// CAR TO FLOOR
bool BoxToBox(IModel* Car, float CarWidth, float CarHeight, IModel* Floor, float FloorWidth, float FloorHeight)
{
	float CombinedWidth = (CarWidth * 0.5f) + FloorWidth * 0.5f;
	float CombinedHeight = (CarHeight * 0.5f) + FloorHeight * 0.5f;

	if (Car->GetX() >= Floor->GetX() - CombinedWidth && Car->GetX() <= Floor->GetX() + CombinedWidth &&
		Car->GetZ() >= Floor->GetZ() - CombinedHeight && Car->GetZ() <= Floor->GetZ() + CombinedHeight)
	{
		return true;
	}
	return false;
}

// FROG TO FLOOR
bool SphereToBox(IModel* Frog, float SphereRadius, IModel* Floor, float FloorWidth, float FloorHeight)
{
	float CombinedWidth = SphereRadius + FloorWidth * 0.5f;
	float CombinedHeight = SphereRadius + FloorHeight * 0.5f;

	if (Frog->GetX() >= Floor->GetX() - CombinedWidth && Frog->GetX() <= Floor->GetX() + CombinedWidth &&
		Frog->GetZ() >= Floor->GetZ() - CombinedHeight && Frog->GetZ() <= Floor->GetZ() + CombinedHeight)
	{
		return false;
	}
	return true;
}