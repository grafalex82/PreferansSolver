#include <iostream>
#include <time.h>

#include "CardPack.h"
#include "CardDefs.h"
#include "GameState.h"
#include "Player.h"
#include "Path.h"


int main()
{

    // A famous "`Kovalevska's miser" game
    CGameState game(CPlayer("J^ Q^   7+ 9+   1$ J$ Q$ K$   7@ J@", PS_P2MAX),
                    CPlayer("7^ 8^ 9^ 1^   8+   7$ 8$ 9$   8@ 9@", PS_P2MIN),
                    CPlayer("K^ A^   1+ J+ Q+   A$   1@ Q@ K@ A@", PS_P2MAX));

    //game.setTrumpSuit(CS_SPIDES);

    std::cout << game << std::endl;

    std::cout << "Searching a solution for Kovalevska's miser..." << std::endl;
    clock_t tStart = clock();
    CPath path = game.playGameRecursive();
    clock_t tStop = clock();

    std::cout << "Whole tree traversed in " << static_cast<double>(tStop - tStart)/CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "The optimal path is: " << path.getOptimalPath() << std::endl;
    std::cout << "The optimal score is: " << path.getOptimalScore() << std::endl;

    return 0;
}
