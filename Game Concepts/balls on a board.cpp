// balls on a board.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("S:\\TL-Engine\\Media");

	// CAMERA SETUP
	ICamera* MyCamera;
	MyCamera = myEngine->CreateCamera(kManual);
	MyCamera->SetPosition(0.0f, 240.0f, -240.0f);
	MyCamera->RotateLocalX(45.0f);

	// MESHES AND MODELS USED
	IMesh* FloorMesh = myEngine->LoadMesh("Floor.x");
	IModel* Floor = FloorMesh->CreateModel(0.0f, -0.2f, 0.0f);

	IMesh* BoardMesh = myEngine->LoadMesh("Board.x");
	IModel* Board = BoardMesh->CreateModel(0.0f, 0.0f, 0.0f);

	IMesh* Ball_1Mesh = myEngine->LoadMesh("Ball.x");
	IModel* Ball_1 = Ball_1Mesh->CreateModel(125.0f, 10.0f, -125.0f);
	Ball_1->SetSkin("violet.png");

	IMesh* Ball_2Mesh = myEngine->LoadMesh("Ball.x");
	IModel* Ball_2 = Ball_2Mesh->CreateModel(-125.0f, 10.0f, 75.0f);
	Ball_2->SetSkin("blue.png");

	IMesh* Ball_3Mesh = myEngine->LoadMesh("Ball.x");
	IModel* Ball_3 = Ball_3Mesh->CreateModel(-125.0f, 10.0f, -125.0f);
	Ball_3->SetSkin("green.png");

	IMesh* Ball_4Mesh = myEngine->LoadMesh("Ball.x");
	IModel* Ball_4 = Ball_4Mesh->CreateModel(125.0f, 10.0f, 75.0f);
	Ball_4->SetSkin("indigo.png");

	// ENUMS,FLOATS AND VARIABLES
	enum EDirection { Up, Down, Left, Right };
	EDirection Ball_1_Direction = Left;
	EDirection Ball_2_Direction = Right;
	EDirection Ball_3_Direction = Up;
	EDirection Ball_4_Direction = Down;

	enum EBallState { Not_Moving, Moving };
	EBallState Ball_1_State = Moving;
	EBallState Ball_2_State = Not_Moving;
	EBallState Ball_3_State = Not_Moving;
	EBallState Ball_4_State = Moving;

	enum EBallHeightState { Grounded, Moving_Down, Moving_Up, Raised };
	EBallHeightState Height_State = Grounded;

	enum EBall_Colour { Violet, Green, Blue, Indigo };
	EBall_Colour Ball_1_colour = Violet;
	EBall_Colour Ball_2_colour = Blue;
	EBall_Colour Ball_3_colour = Green;
	EBall_Colour Ball_4_colour = Indigo;

	const float kGameSpeed = 0.10f;
	float SphereSpeed = 1.0f;

	float MaxSpeed = kGameSpeed * 5;
	float MinSpeed = kGameSpeed / 2;

	const float kTurningPoint_1 = 125.0f;
	const float kTurningPoint_2 = 75.0f;
	const float kTurningPoint_3 = 40.0f;
	const float kTurningPoint_4 = 10.0f;

	const float kGamePause = 0.0f;
	bool PauseGame = false;

	float MouseWheelResult = 0.0f;


	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		// PAUSES GAME AND STOPS BALLS FROM MOVING UP,DOWN OR IN A SQUARE
		if (myEngine->KeyHit(Key_P))
		{
			if (PauseGame == false)
			{
				PauseGame = true;
			}
			else if (PauseGame == true)
			{
				Ball_1->MoveX(kGamePause);
				Ball_1->MoveZ(kGamePause);
				Ball_2->MoveX(kGamePause);
				Ball_2->MoveZ(kGamePause);
				Ball_3->MoveX(kGamePause);
				Ball_3->MoveZ(kGamePause);
				Ball_4->MoveX(kGamePause);
				Ball_4->MoveZ(kGamePause);
				PauseGame = false;
			}
		}

		if (PauseGame == false)
		{

			// MOUSEWHEELRESULT RECORDS THE RESULT OF "GETMOUSEWHEELMOVEMENT" DUE TO NOT BEING ABLE TO RECORD MULTIPLE INSTANCES OF SCROLLING THE WHEEL
			// MOUSEWHEELRESULT WATCHES FOR CHANGES OF THE MOUSE WHEEL (INCREASE/DECREASE)
			// ONCE INCREASED OR DECREASED IT WILL ONLY EVER GO TO ITS MAXIMUM OR MINIMUM SPEED
			MouseWheelResult = myEngine->GetMouseWheelMovement();
			if (MouseWheelResult < 0.0f)
			{
				SphereSpeed -= kGameSpeed / 10.0f;
				if (SphereSpeed <= MinSpeed)
				{
					SphereSpeed = MinSpeed;
				}
			}
			else if (MouseWheelResult > 0.0f)
			{
				SphereSpeed += kGameSpeed / 10.0f;
				if (SphereSpeed >= MaxSpeed)
				{
					SphereSpeed = MaxSpeed;
				}
			}

			// CHECKS BALL STATE (MOVING / NOT_MOVING)
			// CHECKS BALL DIRECTION
			// MOVES BALL UNTIL DESTINATION
			// CHANGES BALL TO NEXT DIRECTION
			// STOPS ROTATION IF BALL ISNT GROUNDED
			// CHANGES DIRECTION FOR NEXT INSTANCE
			if (Ball_1_State == Moving)
			{
				if (Ball_1_Direction == Left)
				{
					Ball_1->MoveX(-kGameSpeed * SphereSpeed);
					if (Height_State == Grounded)Ball_1->RotateZ(kGameSpeed * SphereSpeed);
					if (Ball_1->GetX() <= -kTurningPoint_2)
					{
						Ball_1_Direction = Up;
					}
				}

				// CHECKS BALL STATE (MOVING / NOT_MOVING)
				// CHECKS BALL DIRECTION
				// MOVES BALL UNTIL DESTINATION
				// CHANGES BALL TO NEXT DIRECTION
				// STOPS ROTATION IF BALL ISNT GROUNDED
				// CHANGES DIRECTION FOR NEXT INSTANCE
				if (Ball_1_Direction == Up)
				{
					Ball_1->MoveZ(kGameSpeed * SphereSpeed);
					if (Height_State == Grounded)Ball_1->RotateX(kGameSpeed * SphereSpeed);
					if (Ball_1->GetZ() >= kTurningPoint_2)
					{
						Ball_1_Direction = Right;
					}
				}

				// CHECKS BALL STATE (MOVING / NOT_MOVING)
				// CHECKS BALL DIRECTION
				// MOVES BALL UNTIL DESTINATION
				// CHANGES BALL TO NEXT DIRECTION
				// STOPS ROTATION IF BALL ISNT GROUNDED
				// CHANGES DIRECTION FOR NEXT INSTANCE
				if (Ball_1_Direction == Right)
				{
					Ball_1->MoveX(kGameSpeed * SphereSpeed);
					if (Height_State == Grounded)Ball_1->RotateZ(-kGameSpeed * SphereSpeed);
					if (Ball_1->GetX() >= kTurningPoint_1)
					{
						Ball_1_Direction = Down;
					}
				}

				// CHECKS BALL STATE (MOVING / NOT_MOVING)
				// CHECKS BALL DIRECTION
				// MOVES BALL UNTIL DESTINATION
				// CHANGES BALL TO NEXT DIRECTION
				// STOPS ROTATION IF BALL ISNT GROUNDED
				// CHANGES DIRECTION FOR NEXT INSTANCE
				if (Ball_1_Direction == Down)
				{
					Ball_1->MoveZ(-kGameSpeed * SphereSpeed);
					if (Height_State == Grounded)Ball_1->RotateX(-kGameSpeed * SphereSpeed);
					if (Ball_1->GetZ() <= -kTurningPoint_1)
					{
						Ball_1_Direction = Left;
						Ball_1_State = Not_Moving;
						Ball_4_State = Not_Moving;
						Ball_2_State = Moving;
						Ball_3_State = Moving;
					}
				}
			}


			// CHECKS BALL STATE (MOVING / NOT_MOVING)
			// CHECKS BALL DIRECTION
			// MOVES BALL UNTIL DESTINATION
			// CHANGES BALL TO NEXT DIRECTION
			// STOPS ROTATION IF BALL ISNT GROUNDED
			// CHANGES DIRECTION FOR NEXT INSTANCE
			if (Ball_2_State == Moving)
			{
				if (Ball_2_Direction == Right)
				{
					Ball_2->MoveX(kGameSpeed * SphereSpeed);
					if (Height_State == Grounded)Ball_2->RotateZ(-kGameSpeed * SphereSpeed);
					if (Ball_2->GetX() >= kTurningPoint_2)
					{
						Ball_2_Direction = Down;
					}
				}

				// CHECKS BALL STATE (MOVING / NOT_MOVING)
				// CHECKS BALL DIRECTION
				// MOVES BALL UNTIL DESTINATION
				// CHANGES BALL TO NEXT DIRECTION
				// STOPS ROTATION IF BALL ISNT GROUNDED
				// CHANGES DIRECTION FOR NEXT INSTANCE
				if (Ball_2_Direction == Down)
				{
					Ball_2->MoveZ(-kGameSpeed * SphereSpeed);
					if (Height_State == Grounded)Ball_2->RotateX(-kGameSpeed * SphereSpeed);
					if (Ball_2->GetZ() <= -kTurningPoint_1)
					{
						Ball_2_Direction = Left;
					}
				}

				// CHECKS BALL STATE (MOVING / NOT_MOVING)
				// CHECKS BALL DIRECTION
				// MOVES BALL UNTIL DESTINATION
				// CHANGES BALL TO NEXT DIRECTION
				// STOPS ROTATION IF BALL ISNT GROUNDED
				// CHANGES DIRECTION FOR NEXT INSTANCE
				if (Ball_2_Direction == Left)
				{
					Ball_2->MoveX(-kGameSpeed * SphereSpeed);
					if (Height_State == Grounded)Ball_2->RotateZ(kGameSpeed * SphereSpeed);
					if (Ball_2->GetX() <= -kTurningPoint_1)
					{
						Ball_2_Direction = Up;
					}
				}

				// CHECKS BALL STATE (MOVING / NOT_MOVING)
				// CHECKS BALL DIRECTION
				// MOVES BALL UNTIL DESTINATION
				// CHANGES BALL TO NEXT DIRECTION
				// STOPS ROTATION IF BALL ISNT GROUNDED
				// CHANGES DIRECTION FOR NEXT INSTANCE
				if (Ball_2_Direction == Up)
				{
					Ball_2->MoveZ(kGameSpeed * SphereSpeed);
					if (Height_State == Grounded)Ball_2->RotateX(kGameSpeed * SphereSpeed);
					if (Ball_2->GetZ() >= kTurningPoint_2)
					{
						Ball_2_Direction = Right;
						Ball_2_State = Not_Moving;
						Ball_3_State = Not_Moving;
						Ball_1_State = Moving;
						Ball_4_State = Moving;
					}
				}
			}

			// CHECKS BALL STATE (MOVING / NOT_MOVING)
			// CHECKS BALL DIRECTION
			// MOVES BALL UNTIL DESTINATION
			// CHANGES BALL TO NEXT DIRECTION
			// STOPS ROTATION IF BALL ISNT GROUNDED
			// CHANGES DIRECTION FOR NEXT INSTANCE
			if (Ball_3_State == Moving)
			{

				if (Ball_3_Direction == Up)
				{
					Ball_3->MoveZ(kGameSpeed * SphereSpeed);
					if (Height_State == Grounded)Ball_3->RotateX(kGameSpeed * SphereSpeed);
					if (Ball_3->GetZ() >= kTurningPoint_2)
					{
						Ball_3_Direction = Right;
					}
				}

				// CHECKS BALL STATE (MOVING / NOT_MOVING)
				// CHECKS BALL DIRECTION
				// MOVES BALL UNTIL DESTINATION
				// CHANGES BALL TO NEXT DIRECTION
				// STOPS ROTATION IF BALL ISNT GROUNDED
				// CHANGES DIRECTION FOR NEXT INSTANCE
				if (Ball_3_Direction == Right)
				{
					Ball_3->MoveX(kGameSpeed * SphereSpeed);
					if (Height_State == Grounded)Ball_3->RotateZ(-kGameSpeed * SphereSpeed);
					if (Ball_3->GetX() >= kTurningPoint_2)
					{
						Ball_3_Direction = Down;
					}
				}

				// CHECKS BALL STATE (MOVING / NOT_MOVING)
				// CHECKS BALL DIRECTION
				// MOVES BALL UNTIL DESTINATION
				// CHANGES BALL TO NEXT DIRECTION
				// STOPS ROTATION IF BALL ISNT GROUNDED
				// CHANGES DIRECTION FOR NEXT INSTANCE
				if (Ball_3_Direction == Down)
				{
					Ball_3->MoveZ(-kGameSpeed * SphereSpeed);
					if (Height_State == Grounded)Ball_3->RotateX(-kGameSpeed * SphereSpeed);
					if (Ball_3->GetZ() <= -kTurningPoint_1)
					{
						Ball_3_Direction = Left;
					}
				}

				// CHECKS BALL STATE (MOVING / NOT_MOVING)
				// CHECKS BALL DIRECTION
				// MOVES BALL UNTIL DESTINATION
				// CHANGES BALL TO NEXT DIRECTION
				// STOPS ROTATION IF BALL ISNT GROUNDED
				// CHANGES DIRECTION FOR NEXT INSTANCE
				if (Ball_3_Direction == Left)
				{
					Ball_3->MoveX(-kGameSpeed * SphereSpeed);
					if (Height_State == Grounded)Ball_3->RotateZ(kGameSpeed * SphereSpeed);
					if (Ball_3->GetX() <= -kTurningPoint_1)
					{
						Ball_3_Direction = Up;
					}
				}
			}

			// CHECKS BALL STATE (MOVING / NOT_MOVING)
			// CHECKS BALL DIRECTION
			// MOVES BALL UNTIL DESTINATION
			// CHANGES BALL TO NEXT DIRECTION
			// STOPS ROTATION IF BALL ISNT GROUNDED
			// CHANGES DIRECTION FOR NEXT INSTANCE
			if (Ball_4_State == Moving)
			{

				if (Ball_4_Direction == Down)
				{
					Ball_4->MoveZ(-kGameSpeed * SphereSpeed);
					if (Height_State == Grounded)Ball_4->RotateX(-kGameSpeed * SphereSpeed);
					if (Ball_4->GetZ() <= -kTurningPoint_1)
					{
						Ball_4_Direction = Left;
					}
				}

				// CHECKS BALL STATE (MOVING / NOT_MOVING)
				// CHECKS BALL DIRECTION
				// MOVES BALL UNTIL DESTINATION
				// CHANGES BALL TO NEXT DIRECTION
				// STOPS ROTATION IF BALL ISNT GROUNDED
				// CHANGES DIRECTION FOR NEXT INSTANCE
				if (Ball_4_Direction == Left)
				{
					Ball_4->MoveX(-kGameSpeed * SphereSpeed);
					if (Height_State == Grounded)Ball_4->RotateZ(kGameSpeed * SphereSpeed);
					if (Ball_4->GetX() <= -kTurningPoint_2)
					{
						Ball_4_Direction = Up;
					}
				}

				// CHECKS BALL STATE (MOVING / NOT_MOVING)
				// CHECKS BALL DIRECTION
				// MOVES BALL UNTIL DESTINATION
				// CHANGES BALL TO NEXT DIRECTION
				// STOPS ROTATION IF BALL ISNT GROUNDED
				// CHANGES DIRECTION FOR NEXT INSTANCE
				if (Ball_4_Direction == Up)
				{
					Ball_4->MoveZ(kGameSpeed * SphereSpeed);
					if (Height_State == Grounded)Ball_4->RotateX(kGameSpeed * SphereSpeed);
					if (Ball_4->GetZ() >= kTurningPoint_2)
					{
						Ball_4_Direction = Right;
					}
				}

				// CHECKS BALL STATE (MOVING / NOT_MOVING)
				// CHECKS BALL DIRECTION
				// MOVES BALL UNTIL DESTINATION
				// CHANGES BALL TO NEXT DIRECTION
				// STOPS ROTATION IF BALL ISNT GROUNDED
				// CHANGES DIRECTION FOR NEXT INSTANCE
				if (Ball_4_Direction == Right)
				{
					Ball_4->MoveX(kGameSpeed * SphereSpeed);
					if (Height_State == Grounded)Ball_4->RotateZ(-kGameSpeed * SphereSpeed);
					if (Ball_4->GetX() >= kTurningPoint_1)
					{
						Ball_4_Direction = Down;
					}
				}
			}

			// HEIGHT_STATE IS SET TO GROUNDED STANDARD (THE BALLS ARE ON THE GROUND)
			// WHEN "KEY_UP" IS PRESSED THIS CHANGES THE HEIGHT_STATE
			if (Height_State == Grounded)
			{
				if (myEngine->KeyHit(Key_Up))
				{
					Height_State = Moving_Up;
				}
			}

			// AFTER THE HEIGHT_STATE HAS CHANGED TO MOVING_UP THE 4 BALLS ARE RAISED
			// 1 OF THE BALLS IS CHECKED SO IT STOPS WHEN IT REACHS 30 UNITS HIGHER THAN ITS STARTING POSITION
			// AFTER REACHING ITS MAXIUM HEIGHT OF 40 UNITS TOTAL THE HEIGHT_STATE IS CHANGED TO RAISED
			if (Height_State == Moving_Up)
			{
				Ball_1->MoveY(kGameSpeed * SphereSpeed);
				Ball_2->MoveY(kGameSpeed * SphereSpeed);
				Ball_3->MoveY(kGameSpeed * SphereSpeed);
				Ball_4->MoveY(kGameSpeed * SphereSpeed);
				if (Ball_1->GetY() >= kTurningPoint_3)
				{
					Height_State = Raised;
				}
			}

			// CHECKS HEIGHT_STATE
			// CHANGES THE HEIGHT_STATE AFTER "KEY_DOWN" IS PRESSED
			if (Height_State == Raised)
			{
				if (myEngine->KeyHit(Key_Down))
				{
					Height_State = Moving_Down;
				}
			}

			// AFTER THE HEIGHT_STATE HAS CHANGED TO MOVING_DOWN THE 4 BALLS ARE MOVED DOWN TO THEIR STARTING POSITION
			// 1 BALL IS CHECKED SO IT STOPS WHEN IT REACHES ITS LOWEST UNIT (10.0F)
			// AFTER REACHING ITS ORIGINAL POSTION THE HEIGHT_STATE IS CHANGED BACK TO GROUNDED
			if (Height_State == Moving_Down)
			{
				Ball_1->MoveY(-kGameSpeed * SphereSpeed);
				Ball_2->MoveY(-kGameSpeed * SphereSpeed);
				Ball_3->MoveY(-kGameSpeed * SphereSpeed);
				Ball_4->MoveY(-kGameSpeed * SphereSpeed);
				if (Ball_1->GetY() <= kTurningPoint_4)
				{
					Height_State = Grounded;
				}
			}

			//BALL COLOUR CYCLE
			if (Ball_1_colour == Violet && Ball_2_colour == Blue && Ball_3_colour == Green && Ball_4_colour == Indigo)
			{
				if (myEngine->KeyHit(Key_E))
				{
					Ball_1_colour = Indigo;
					Ball_1->SetSkin("indigo.png");

					Ball_2_colour = Green;
					Ball_2->SetSkin("green.png");

					Ball_3_colour = Violet;
					Ball_3->SetSkin("violet.png");

					Ball_4_colour = Blue;
					Ball_4->SetSkin("blue.png");
				}
			}

			if (Ball_1_colour == Indigo && Ball_2_colour == Green && Ball_3_colour == Violet && Ball_4_colour == Blue)
			{
				if (myEngine->KeyHit(Key_E))
				{
					Ball_1_colour = Blue;
					Ball_1->SetSkin("blue.png");

					Ball_2_colour = Violet;
					Ball_2->SetSkin("violet.png");

					Ball_3_colour = Indigo;
					Ball_3->SetSkin("indigo.png");

					Ball_4_colour = Green;
					Ball_4->SetSkin("green.png");
				}
			}

			if (Ball_1_colour == Blue && Ball_2_colour == Violet && Ball_3_colour == Indigo && Ball_4_colour == Green)
			{
				if (myEngine->KeyHit(Key_E))
				{
					Ball_1_colour = Green;
					Ball_1->SetSkin("green.png");

					Ball_2_colour = Indigo;
					Ball_2->SetSkin("indigo.png");

					Ball_3_colour = Blue;
					Ball_3->SetSkin("blue.png");

					Ball_4_colour = Violet;
					Ball_4->SetSkin("violet.png");
				}
			}

			if (Ball_1_colour == Green && Ball_2_colour == Indigo && Ball_3_colour == Blue && Ball_4_colour == Violet)
			{
				if (myEngine->KeyHit(Key_E))
				{
					Ball_1_colour = Violet;
					Ball_1->SetSkin("violet.png");

					Ball_2_colour = Blue;
					Ball_2->SetSkin("blue.png");

					Ball_3_colour = Green;
					Ball_3->SetSkin("green.png");

					Ball_4_colour = Indigo;
					Ball_4->SetSkin("indigo.png");
				}
			}
		}

		// EXIT
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}