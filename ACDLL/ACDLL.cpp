// ACDLL.cpp : Définit les fonctions exportées de la DLL.
//

#include "pch.h"
#include "framework.h"
#include "ACDLL.h"


// Il s'agit d'un exemple de variable exportée
ACDLL_API int nACDLL=0;

// Il s'agit d'un exemple de fonction exportée.
ACDLL_API int fnACDLL(void)
{
    return 0;
}

// Il s'agit du constructeur d'une classe qui a été exportée.
CACDLL::CACDLL()
{
    return;
}
