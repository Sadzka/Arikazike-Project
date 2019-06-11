#include "game.hpp"
#include <fstream>


int main(int argc, void** argv[])
{
    #ifdef __DEBUG
    std::cout<<"GCC: "<<__GNUC__<<"."<<__GNUC_MINOR__<<"."<<__GNUC_PATCHLEVEL__<<std::endl;
    {
        std::cout << "argc: " << argc << std::endl;
        for (int i = 0; i < argc; i++ )
        {
            std::cout << "argv: " << argv[i] << std::endl;
        }

        std::fstream file;
        file.open("counter.txt", std::ios::out | std::ios::in );
        if(file.good())
        {
            int counter;
            file >> counter;
            std::cout << "Kompilacja numer: " << ++counter << std::endl;
            file.seekg( 0, std::ios::beg );
            file << counter;
            file.close();
        }
    }
    #endif // __DEBUG

    Game game;
    while(!game.getWindow()->isClosed())
    {
        game.update();
        game.render();
        game.lateUpdate();
    }

    return 0;
}
