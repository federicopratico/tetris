#include <iostream>
#include <fstream>
#include <string>
#include "tetris.hpp"
#include <vector>
#include <cstdlib>
#include <sstream>


// int main()
// {
// 	std::ifstream ifs;
// 	std::ofstream ofs;
// 	ifs.open("input.txt");
// 	ofs.open("output.txt");


// 	 try{
// 	 	tetris t;
// 	 	ifs >> t;
// 	 	t.print_ascii_art(std::cout);
// 	 	ofs << t;

// 	 	int ins = 4;
// 	 	while(ins--){
// 	 		int x;
// 	 		piece p;
// 	 		std::cout << "inserire pezzo" << std::endl;
// 	 		std::cin >> p;
// 	 		p.print_ascii_art(std::cout);
// 	 		std::cout << std::endl;

// 	 		int rotazione = 0;
// 	 		std::cout << "vuoi ruotare il pezzo? quante volte vuoi ruotare il pezzo?\n";
// 	 		std::cin >> rotazione;
// 	 		while(rotazione > 0){
// 	 			p.rotate();
// 	 			p.print_ascii_art(std::cout);
// 	 			rotazione--;
// 	 		}

	 		
// 	 		std::cout << std::endl;
// 	 		std::cout << "inserire coordinate x" << std::endl;
// 	 		std::cin >> x;

// 	 		t.insert(p,x);
// 	 		t.print_ascii_art(std::cout);
// 	 		std::cout << std::endl;
	 		
// 	 		ofs << "tetris " << 4-ins << std::endl;
// 	 		ofs << t;
// 	 	}
// 	 }catch(tetris_exception ex){
// 	 	std::cout << ex.what();
// 	 }




// 	ifs.close();
// 	ofs.close();
// 	return 0;
// }



int main(){
    try{
        std::ifstream file("input_tetris.txt");
        tetris t;
        file >> t;
        t.print_ascii_art(std::cout);

        std::istringstream is("2 202 () 2 202 ()");
        piece p;
        is >> p;
        p.print_ascii_art(std::cout);
        t.insert(p, 0);
        t.print_ascii_art(std::cout);
        std::cout << t;

        std::istringstream is1("4 100 ((()()[][])(()[]()[])[][])");
        is1 >> p;
        p.print_ascii_art(std::cout);
        t.insert(p, 0);
        t.print_ascii_art(std::cout);
        std::cout << t;

        std::istringstream is2("4 100 ([][](()()[][])(()[]()[]))");
        is2 >> p;
        p.rotate();
        p.rotate();
        p.print_ascii_art(std::cout);
        t.insert(p, 4);
        t.print_ascii_art(std::cout);
        std::cout << t;

        //std::istringstream is("2 202 ()");
        is >> p;
        p.print_ascii_art(std::cout);
        t.insert(p, 6);
        t.print_ascii_art(std::cout);
        std::cout << t;

        std::istringstream is3("4 32 (([][]()())[](()[]()[])[])");
        is3 >> p;
        p.print_ascii_art(std::cout);
        t.insert(p, 4);
        t.print_ascii_art(std::cout);
        std::cout << t;


        /*std::istringstream is4("2 11 ()");
        is4 >> p;
        p.print_ascii_art(std::cout);
        t.insert(p, 5);
        t.print_ascii_art(std::cout);
        std::cout << t;*/

        std::istringstream is5("2 11 ()");
        is5 >> p;
        p.print_ascii_art(std::cout);
        t.insert(p, 0);
        t.print_ascii_art(std::cout);
        std::cout << t;

        std::istringstream is6("1 11 ()");
        is6 >> p;
        p.print_ascii_art(std::cout);
        t.insert(p, 2);
        t.print_ascii_art(std::cout);
        std::cout << t;

        std::istringstream is7("8 123 ([][](([][]()())([][]()())()())[])");
        is7 >> p;
        p.print_ascii_art(std::cout);
        t.insert(p, 3);
        t.print_ascii_art(std::cout);
        std::cout << t;

        std::istringstream is9("8 123 []");
        is9 >> p;
        p.print_ascii_art(std::cout);
        bool c = t.containment(p, 3, 4);
        if(c){
            std::cout << "il pezzo viene contenuto\n";
        }else{
            std::cout << "il pezzo non viene contenuto\n";
        }
        t.insert(p, 3);
        t.print_ascii_art(std::cout);
        std::cout << t;



        std::istringstream is8("8 123 ([][](([][]()())([][]()())()())[])");
        is8 >> p;
        p.print_ascii_art(std::cout);
        t.insert(p, 3);
        t.print_ascii_art(std::cout);
        std::cout << t;






        /*std::istringstream is8("8 28 ([][][]([][][]([]()[][])))");
        is8 >> p;
        p.print_ascii_art(std::cout);
        t.insert(p, 0);
        t.print_ascii_art(std::cout);
        std::cout << t;

        std::istringstream is9("8 29 ([][][]([][][]([]()[][])))");
        is9 >> p;
        p.print_ascii_art(std::cout);
        t.insert(p, 0);
        t.print_ascii_art(std::cout);
        std::cout << t;

        std::istringstream is10("8 30 ([][][]([][][]([]()[][])))");
        is10 >> p;
        p.print_ascii_art(std::cout);
        t.insert(p, 0);
        t.print_ascii_art(std::cout);
        std::cout << t;

        std::istringstream is11("8 31 ([][][]([][][]([]()[][])))");
        is11 >> p;
        p.print_ascii_art(std::cout);
        t.insert(p, 0);
        t.print_ascii_art(std::cout);
        std::cout << t;

        std::istringstream is12("8 32 ([][][]([][][]([]()[][])))");
        is12 >> p;
        p.print_ascii_art(std::cout);
        t.insert(p, -7);
        t.print_ascii_art(std::cout);
        std::cout << t;*/

        /*std::istringstream is11("4 15 (([][]()())([][]()[])[](()[]()[]))");
        is11 >> p;
        p.print_ascii_art(std::cout);
        t.insert(p, 0);
        t.print_ascii_art(std::cout);
        std::cout << t;*/
    }
    catch(tetris_exception e){
        std::cout << e.what();
    }
}


// int main()
// {
// 	std::ifstream ifs;
// 	std::ofstream ofs;
// 	ifs.open("input.txt");
// 	ofs.open("output.txt");


// 	 try{
// 	 	tetris t;
	 	
// 	 	tetris t2;

// 	 	ifs >> t;
// 	 	t.print_ascii_art(std::cout);
// 	 	ofs << t;

	 	
// 	 	t2 = std::move(t);
// 	 	tetris t1 = std::move(t);

	 	

// 	 	t1.print_ascii_art(std::cout);
// 	 	t2.print_ascii_art(std::cout);

// 	 	ofs << "MOVE CONSTRUCTOR\n" << t1 << "MOVE ASSIGNMENT\n" << t2 << "TETRIS ORIGINALE\n" << t;


// 	 }catch(tetris_exception ex){
// 	 	std::cout << ex.what();
// 	 }




// 	ifs.close();
// 	ofs.close();
// 	return 0;
// }
