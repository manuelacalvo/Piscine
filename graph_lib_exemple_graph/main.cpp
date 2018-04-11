#include "grman/grman.h"
#include <iostream>
#include "graph.h"


int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du r�pertoire o� se trouvent les images � charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g;
    ///choix du monde
    int sauv = 0;
    ///choix new ou sauv�
    int choix2=0;
    int world;


    ///Tant que l'utilisateur ene quitte pas et rentre les bonnes donn�es
    while(g.GetMonde() != 4 || (g.GetMonde()!=1 && g.GetMonde() !=2 && g.GetMonde()!=3) )

    {

        std::cout<<" choississez le reseau que vous souhaitez"<<std::endl<<"1. la banquise  2.la savane  3.la foret"<<std::endl;
        std::cin>>world;
        g.SetMonde(world);
        if(g.GetMonde()==1 || g.GetMonde() ==2 || g.GetMonde()==3 )
        {

            std::cout<<" 1. nouveau graphe  2.chargement graphe sauvgarde "<<std::endl;
            std::cin>>choix2;

            ///Appel pour charger les graphes
            g.editer_reseaux(choix2);


            ///Tant qu'on ne quitte pas sur le bouton menu on boucle
            while (g.get_interface()->m_menu.get_value()==false)
            {
                ///Si on appuit sur le bouton quitter
                if(g.get_interface()->m_quitter.get_value()==true)
                {
                    ///On ferme le programme
                    exit(0);
                }

                ///Mise � jour de l'interface graphique
                g.update();

                ///Si on appuit sur le bouton sauvegarde
                if(g.get_interface()->m_sauvegarde.get_value() == true)
                {
                    ///On appel le sous-programme sauvegarde
                    g.sauvegarde();
                }

                ///Si on appuit sur le bouton connexite
                if(g.get_interface()->m_connexite.get_value() == true)
                {
                    ///appel connexite
                    g.colorier_compo_connexe();

                }

                ///Si on appuit sur le bouton simulation1
                if(g.get_interface()->m_animation1.get_value() == true)
                {
                    g.get_interface()->m_animation1.set_value(false);
                    ///appel dynamique de population1
                    g.dynamique_pop1();
                }

                ///Si on appuit sur le bouton simulation2
                if(g.get_interface()->m_animation2.get_value() == true)
                {g.get_interface()->m_animation2.set_value(false);
                    ///appel gestion de population
                    g.gestion();
                }

                ///Si on appuit sur le bouton ajouter sommet
                if(g.get_interface()->m_ajout_s.get_value() == true)
                {
                    ///appel ajouter sommet
                    g.ajout_sommet_utilisateur();
                }

                ///Si on appuit sur le bouton ajouter arete
                if(g.get_interface()->m_ajout_a.get_value()==true)
                {
                    ///appel ajouter arete
                    g.ajout_arete_selec();
                }

                ///Si on appuit sur le bouton forte connexit�
                if(g.get_interface()->m_fort_connexite.get_value()==true)
                {
                    ///On le remet � false
                    g.get_interface()->m_fort_connexite.set_value(false);
                    ///appel fortement connexe
                    g.fortement_connexe();
                }

                ///appel suppresion sommet
                g.supp_sommet_main();

                ///Si on appuit sur le bouton k-connexit�
                if(g.get_interface()->m_k_connexite.get_value()== true)
                {
                    ///On le remet � false
                    g.get_interface()->m_k_connexite.set_value(false);
                    ///appel k_connexit�
                    g.changement_nb_desactive();

                }

                ///Si on appuit sur le bouton graphe r�suit
                if(g.get_interface()->m_graph_reduit.get_value()==true)
                {
                    ///On le remet � false
                    g.get_interface()->m_graph_reduit.set_value(false);
                    ///Enlever graphe r�duit
                    g.vider_graphe_reduit();

                }



                ///appel suppression arete
                g.supp_arete_main();


                /// Mise � jour g�n�rale des outils li�s � l'interface
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

