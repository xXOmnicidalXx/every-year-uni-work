using System;
using System.Collections.Generic;

namespace intro_to_programming_assignment
{
    class Program
    {
        static void Main() // initialises data and shows 'menu'
        {
            string[,] Products = { { "Chocolate Bar", "0.80" }, { "Soda Can", "0.70" }, { "Soda Bottle", "1.25" }, { "Crisps", "0.50" }, { "Cookies", "1.10" } };
            int[] Quantity = { 5, 5, 5, 5, 5 }; // quantity of all products = to 5
            double Credits = 0;
            List<string> Order_List = new List<string>(); // creates order list for basket
            List<double> Price_List = new List<double>(); // creates price list
            MainMenu(Products, Quantity, Credits, Order_List, Price_List); //parameters
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        static void Display(string[,] Products, int[] Quantity) // displays products and quantity parameters
        {
            Console.WriteLine("Product number | Product Name | Product Price | Quantity ");
            for (int i = 0; i != 5; i++)
            {
                Console.WriteLine("{0}.  {1}  {2}  {3}", i + 1, Products[i, 0], Products[i, 1], Quantity[i]);
            }
            Console.WriteLine();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        static double credits(double Credits) // tests, checks and adds credits
        {
            bool Loop = true;
            while (Loop == true)
            {
                Console.WriteLine("Please enter credits(£0.00)"); // starts credits off at £0.00
                Console.WriteLine();
                var Check_D = Console.ReadLine();
                bool Working = Double.TryParse(Check_D, out double num);
                if (Working)
                {
                    Credits += double.Parse(Check_D);
                    Loop = false;
                }
                else
                {
                    Console.WriteLine("'{0}', not a supported format", Check_D);
                }
            }
            Console.WriteLine("{0}", Credits.ToString("c2"));
            return Credits;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        static void Order(string[,] Products, int[] Quantity, double Credits, List<string> Order_List, List<double> Price_List)
        {
            Display(Products, Quantity);

            bool loop = true;
            while (loop == true)
            {
                Console.WriteLine("please choose a product for purchase, enter product number here: ");

                var Check_I = Console.ReadLine();
                int Chosen = 0;
                bool Success = Int32.TryParse(Check_I, out int number); // checks data type
                if (Success)
                {
                    Chosen = int.Parse(Check_I) - 1;

                    if (Quantity[Chosen] <= 0)
                    {
                        Console.WriteLine("product out of stock"); // shows the item is less then 5 or out of stock
                    }
                    else
                    {
                        Order_List.Add(Products[Chosen, 0]);
                        Price_List.Add(double.Parse(Products[Chosen, 1]));
                        Quantity[Chosen] -= 1;
                        string Options;
                        bool loop2 = true;
                        while (loop2 == true)
                        {
                            Console.WriteLine("do you wish you continue shopping?(y/n)"); // choices if you wish to buy 1 or multiple products
                            Options = Console.ReadLine().ToLower();
                            if (Options == "y")
                            {
                                loop2 = false;
                            }
                            if (Options == "n")
                            {
                                loop = false;
                                loop2 = false;
                            }
                            else if (Options != "n" && Options != "y")
                            {
                                Console.WriteLine("invaild option"); // invaild option from choices
                            }
                        }
                    }
                }
            }
            Basket(Products, Quantity, Credits, Order_List, Price_List);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        static void Basket(string[,] Products, int[] Quantity, double Credits, List<string> Order_List, List<double> Price_List)
        {
            if (Order_List.Count == 0)
            {
                Console.WriteLine("basket is empty"); // this shows the baskets current count
            }
            else
            {
                Console.WriteLine("product    price"); // this shows whats in your basket
                double Total = 0;

                for (int item = 0; item < Order_List.Count; item++)
                {
                    Console.WriteLine("{0}  {1}", Order_List[item], Price_List[item]);
                    Total += Price_List[item];
                }
                Console.WriteLine("\n--------------------\n Total: {0}", Total.ToString("c2"));
                bool loop = true;
                while (loop == true)
                {
                    string Answer;
                    Console.WriteLine("do you wish to purchase? (y/n)"); // this is the choice of if you wish to buy whats in the basket
                    Answer = Console.ReadLine().ToLower();
                    if (Answer == "y")
                    {
                        if (Credits < Total)
                        {
                            Console.WriteLine("insuficent funds"); // this shows you dont have enough money for purchasing
                            bool loop_1 = true;
                            while (loop_1 == true)
                            {
                                string Answer_1;
                                Console.WriteLine("do you wish to add credits? (y/n)"); // this is if you wish to add funds/credits (choice)
                                Answer_1 = Console.ReadLine().ToLower();
                                if (Answer_1 == "y")
                                {
                                    Credits = credits(Credits);
                                    loop_1 = false;
                                }
                                if (Answer_1 == "n")
                                {
                                    Console.WriteLine("please add credits to continue shopping");
                                }
                                else if (Answer_1 != "y" && Answer_1 != "n")
                                {
                                    Console.WriteLine("invaild option"); // invaild option choice between Y or N
                                }
                            }
                        }
                        else if (Credits >= Total)
                        {
                            loop = false;
                        }
                    }
                    if (Answer == "n")
                    {
                        for (int i = 0; i == Quantity.Length; i++)
                        {
                            Quantity[i] = 5; // resets basket and adds inventory back to the shelf
                        }
                        Order_List.Clear();
                        Price_List.Clear();
                        Console.WriteLine("inventory reset");
                        MainMenu(Products, Quantity, Credits, Order_List, Price_List);
                        loop = false;
                    }
                    else if (Answer != "y" && Answer != "n")
                    {
                        Console.WriteLine("invaild option"); // invaild option choice between Y or N
                    }
                }
                Credits -= Total;
                Console.WriteLine("balance = {0}, balance being dispenced", Credits.ToString("c2")); // this shows how many credits you have left
                Credits = 0;
                Order_List.Clear();
                Price_List.Clear();

                MainMenu(Products, Quantity, Credits, Order_List, Price_List); // this takes you back to the menu
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        static void MainMenu(string[,] Products, int[] Quantity, double Credits, List<string> Order_List, List<double> Price_List) // variables will update when called/re-called 
        {
            // Display(Products, Quantity);
            bool Loop1 = true;

            while (Loop1 == true)
            {

                string Answer_1;

                // welcoming the user to the vending machine and asking if they wish to continue
                Console.WriteLine("Do you want to see the menu?\n Y / N\n--------------------\n");


                Answer_1 = Console.ReadLine().ToLower(); //changes input to lowercase

                if (Answer_1 == "n")
                {
                    //Console.Clear();
                    Console.WriteLine("thanks for using me, goodbye");
                    Environment.Exit(-1); // this exits the console and stops the process
                    // Console.Clear();
                }
                else if (Answer_1 == "y")
                {
                    Console.WriteLine("--------------------");
                    Display(Products, Quantity);
                    Loop1 = false;
                }
                else
                {
                    Console.WriteLine("please enter the correct option");
                }
            }



            bool Loop = true;
            while (Loop == true)
            {
                Console.WriteLine("--------------------\nLets continue\n--------------------");
                int Option;
                Console.WriteLine("what option would you like to select?\n \n1) Credits >\n2) Order >\n3) Basket >\n\n0) Exit >\n--------------------");

                var Check_I = Console.ReadLine();
                bool Success = Int32.TryParse(Check_I, out int number); // checks data type
                if (Success)
                {
                    Option = int.Parse(Check_I);
                    if (Option == 1)
                    {
                        Credits = credits(Credits);
                        Console.WriteLine("--------------------\nreturning back to menu");
                        //Loop = false;
                    }
                    if (Option == 2)
                    {
                        Order(Products, Quantity, Credits, Order_List, Price_List); // calls order to be used and viewed
                        //Loop = false;
                    }
                    if (Option == 3)
                    {
                        Basket(Products, Quantity, Credits, Order_List, Price_List); // calls basket to be used and viewed
                        //Loop = false;
                    }
                    if (Option == 0)
                    {
                        MainMenu(Products, Quantity, Credits, Order_List, Price_List); // calls menu to be used and viewed
                        //Loop = false;
                    }
                    else if (Option != 0 && Option != 1 && Option != 2 && Option != 3)
                    {
                        Console.WriteLine("invaild option"); // invaild option from the choices given
                    }
                }
                else
                {
                    Console.WriteLine("'{0}', not a supported format", Check_I);
                }
            }
        }
    }
}