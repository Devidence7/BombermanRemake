
IA_in(sf::Vector2i x, Entity_ptr e){
    getMapCoordinates(e->getCenterPosition()) == x;
}


class Move{
    bool verificarInX = true;
    bool verificar_no_InY = true;
    bool verificarExistPath = true;
    bool verificarOnVision()

    void *callback;

    Move(CB);

    bool satisfaceAction(){
        bool satistafe = (!verificarInX || IA_in(x));
        satistafe = satistafe && (!verificar_no_InY || !IA_in(y))
        satisface = satisface && (!verificarExistPath || existPath(x,y))

    }

    ejecutarACtion(){
        callback();
    }
}



// Matar
//    -- Moverse a rango + poner bomba
// Equiparse (buscar powerUp)
    -- PowenUp visible
// Sin Objetivo

//Patrullar
    -- Loop (Go(x,y), Go(y,x))
// **Destruir Muros
    -- Ir a mejor zona posible + poner bomba
// **Esquivar bombas

    -- En Movmimiento
        + Poner bomba 
            P
