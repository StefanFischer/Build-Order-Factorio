tree<Requirement*> tree;

class Requirement {
    enum entityCategory {Factory, Recipe, Technology, Item};
    
    // if Factory: factoryType e.g. "smelting-oven"
    // if Item: item name "coal"
    std::string entityType;
    int numberOfInstances;
}

int main () 
{
    List<Requirement*> Knoten_of_this_level;
    
    while(nicht fertig)
    {
    for(knoten : Knoten_of_this_level)
    {
        switch(knoten.entityCategory)
        {
            case Factory:
                Factory factoryInstance = getFactoryFromJsonByFactorytype(knoten.entityType);
                // anstatt factoryInstance.requirements() muss hier evtl erst eine requirements-
                // liste konstruiert werden, z.B. im Falle des Recipe:
                // - Technology in technology.json finden, die crafting-category freischaltet
                // - ingredients
                hängeKindknotenEin(factoryInstance.requirements());
            case Recipe:
                // Recipe instance aus der anderen Json auslesen, aber auch analog anhand des
                // namens (knoten.entityType)
                // ebenfalls analog: requirements raussuchen und als kindknoten einhängen
            case Technology:
                ...
            case Item:
                // numberOfInstances beachten
        }
    }
    }//while
}

void hängeKindknotenEin (list of requirements...)
{
    if(requirement is a basic item)
        return; // skip this item

    for(all requirements)
        // Checke pfad zur Root, ob dieses item schonmal vorhanden war
        // wenn ja, sind wir in einer Loop! => Fehlerbehandlung
        // - Technologies dürfen mehrfach vorkommen...
        //       !!! Dürfen Recipes und Factories auch mehrfach vorkommen?
        //           ja, oder?
        // - Entweder hier einfach ein anderes Rezept auswählen, das auch
        //   das Item produziert
        // - Oder den Baum zurückgehen bis zum ersten Vorkommen, dieses
        //   duplizierten Items und von dort aus schon mit einem anderen
        //   Rezept / einer anderen Factory / ... weitermachen.

        // einhängen

}

void räumeBaumVonUntenAuf (list<bisherigeEvents> list) {
    if (this technology was already executed)
        skip // we only have to research a technology once, even though it
             // might appear multiple times in the tree

    if(this factory has already been built)
        skip // as we will be building everything sequentially anyways, there
             // is no need to build the same factory multiple times.
}
