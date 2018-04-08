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
    int sauv = 0;
    ///choix new ou sauvé
    int choix2=0;
    int world;
    while(g.GetMonde() != 4 || (g.GetMonde()!=1 && g.GetMonde() !=2 && g.GetMonde()!=3) )

    {

         std::cout<<" choississez le reseau que vous souhaitez ou quitter"<<std::endl<<"1. la banquise  2.la savane  3.la foret  4.quitter"<<std::endl;
    std::cin>>world;
    g.SetMonde(world);
if(g.GetMonde()==1 || g.GetMonde() ==2 || g.GetMonde()==3 )
{



std::cout<<" 1. nouveau graphe  2.chargement graphe sauvgarde "<<std::endl;
std::cin>>choix2;


     g.editer_reseaux(choix2);

    //g.colorier_compo_connexe();

    //g.colorier_compo_fortement_connexe();



//std::cout << "supprimer l'arete 2" << std::endl;
//g.supp_arete(9);


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
while (g.get_interface()->m_menu.get_value()==false)
    {
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
       g.update();

       ///appel sauvegarde
        if(g.get_interface()->m_sauvegarde.get_value() == true)
        {
            g.sauvegarde();
        }

        ///appel connexite
        if(g.get_interface()->m_connexite.get_value() == true)
        {
             g.colorier_compo_connexe();


        }

        ///appel dynamique de population
        if(g.get_interface()->m_animation.get_value() == true)
        {
             g.dynamique_pop1();
        }

       ///appel ajouter sommet
         if(g.get_interface()->m_ajout_s.get_value() == true)
         {
             std::cout<<" dans le if "<<std::endl;
             g.ajout_sommet();


         }

         ///appel ajouter arete
         if(g.get_interface()->m_ajout_a.get_value()==true)
         {
             g.ajout_arete_selec();
         }

         ///appel fortement connexe
        if(g.get_interface()->m_fort_connexite.get_value()==true)
        {
            g.get_interface()->m_fort_connexite.set_value(false);
           g.fortement_connexe();
        }

       ///appel suppresion sommet
        g.supp_sommet_selec();

        ///appel k_connexité
        if(g.get_interface()->m_k_connexite.get_value()== true)
        {
            g.get_interface()->m_k_connexite.set_value(false);
            g.changement_nb_desactive();

        }



    //std::cout<<" avant supp arete "<<std::endl;
        ///appel suppression arete
       g.supp_arete_test();


        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();


    }
    }
    ///blindage saisie
       else
       std::cout<<" choississez parmis l'un des trois mondes proposes svp ... "<<std::endl;
if(g.get_interface()->m_menu.get_value()==false)
        g.get_interface()->m_menu.set_value(true);
    }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


