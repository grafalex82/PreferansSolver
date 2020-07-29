#include <iostream>
#include <time.h>

#include "CardPack.h"
#include "CardDefs.h"
#include "GameState.h"
#include "Player.h"


int main()
{
    clock_t tStart = clock();

    // A famous "`Kovalevska's miser" game
    CGameState game(CPlayer("J^ Q^   7+ 9+   1$ J$ Q$ K$   7@ J@", PS_P2MAX),
                    CPlayer("7^ 8^ 9^ 1^   8+   7$ 8$ 9$   8@ 9@", PS_P2MIN),
                    CPlayer("K^ A^   1+ J+ Q+   A$   1@ Q@ K@ A@", PS_P2MAX));

    game.setTrumpSuit(CS_SPIDES);

    std::cout << game << std::endl;

    CScore initialScore;
    game.setScore(initialScore);
    game.playGameRecursive();
    clock_t tStop = clock();

    std::cout << "Whole tree traversed in " << static_cast<double>(tStop - tStart)/CLOCKS_PER_SEC << " seconds" << std::endl;

    return 0;
}
