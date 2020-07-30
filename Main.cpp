#include <iostream>
#include <time.h>

#include "CardPack.h"
#include "CardDefs.h"
#include "GameState.h"
#include "Player.h"
#include "Path.h"

void playPredefinedGame(CGameState & game, const char * solution) //non-const game
{
    while(*solution != '\0')
    {
        // Skip white spaces
        while(*solution == ' ')
            solution++;

        // parse the card
        Card card = parseCard(solution);
        solution += 2;

        // Make the turn
        std::cout << std::endl << "Player " << game.getActivePlayer() << " is going to play " << getCardStr(card) << std::endl;
        game.makeTurn(card);
        std::cout << "New state is: " << game << std::endl;
    }
}

void searchSolution(CGameState & game)
{
    clock_t tStart = clock();
    CPath path = game.playGameRecursive();
    clock_t tStop = clock();

    std::cout << "Whole tree traversed in " << static_cast<double>(tStop - tStart)/CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "The optimal path is: " << path.getOptimalPath() << std::endl;
    std::cout << "The optimal score is: " << path.getOptimalScore() << std::endl;
}

int main()
{

    // A famous "`Kovalevska's miser" game
    CGameState game(CPlayer("J^ Q^   7+ 9+   1$ J$ Q$ K$   7@ J@", PS_P2MAX),
                    CPlayer("7^ 8^ 9^ 1^   8+   7$ 8$ 9$   8@ 9@", PS_P2MIN),
                    CPlayer("K^ A^   1+ J+ Q+   A$   1@ Q@ K@ A@", PS_P2MAX));

    std::cout << game << std::endl;

#if 1
    std::cout << "Searching a solution for Kovalevska's miser..." << std::endl;
    searchSolution(game);
#else
    //const char * solution = "K$ 9$ A$ 1@ J@ 9@ Q$ 8$ A^ J$ 7$ K^ 1$ 1^ Q+ Q^ 9^ J+ J^ 8^ 1+ 7+ 8+ A@ 8@ K@ 7@ Q@ 9+ 7^";
    playPredefinedGame(game, "K$ 9$ A$ 1@ J@ 9@ Q$ 8$ A^ J$ 7$ K^");
    searchSolution(game);
#endif
    return 0;
}
