#include "grman/grman.h"
#include <iostream>

#include "graph.h"


int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g;
    ///choix du monde
    int choix1=0;
    int sauv = 0;
    ///choix new ou sauvé
    int choix2=0;
    while(choix1 != 4)

    {
         std::cout<<" choississez le reseau que vous souhaitez ou quitter"<<std::endl<<"1. la banquise  2.la savane  3.la foret  4.quitter"<<std::endl;
    std::cin>>choix1;

std::cout<<" 1. nouveau graphe  2.chargement graphe sauvgarde "<<std::endl;
std::cin>>choix2;


     g.editer_reseaux(choix1,choix2);

    //g.colorier_compo_connexe();

    //g.colorier_compo_fortement_connexe();



//std::cout << "supprimer l'arete 2" << std::endl;
//g.supp_arete(9);



sauv=0;
    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
while ( !key[KEY_ESC])
    {
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
       g.update();

        if(g.get_interface()->m_sauvegarde.get_value() == true)
        {   sauv++;


            g.sauvegarde();
        }


        if(g.get_interface()->m_connexite.get_value() == true)
        {
             g.colorier_compo_connexe();
        }
        if(g.get_interface()->m_animation.get_value() == true)
        {
             g.dynamique_pop1();
        }
        if(key[KEY_DOWN])
        {
           g.fortement_connexe();
        }
       g.supp_sommet_selec();
        if(key[KEY_K])
        {
            g.changement_nb_desactive();

        }






       g.supp_arete_test();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();


    }


    }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


