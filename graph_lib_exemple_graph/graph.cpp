#include "graph.h"
#include <fstream>
#include <cstdlib>

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    /// CLICK ME
    m_top_box.add_child( m_legende );
    m_legende.set_message("<- CLICK ME");
    m_legende.set_gravity_y(grman::GravityY::Down );
    /// On ajoute une check-box en bas � gauche
    m_top_box.add_child( m_marche );
    m_marche.set_dim(40, 20);
    m_marche.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down );


    /// On ajoute en haut � gauche du cadre un sous-cadre (une boite dans la boite)
    m_top_box.add_child( m_boite_regles );
    m_boite_regles.set_dim(40, 80);
    m_boite_regles.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up );
    m_boite_regles.set_bg_color(SABLECLAIR);

    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 50);  /// Valeurs arbitraires, � adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}


/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if(m_actif == true)
    {
        if (!m_interface)
            return;

        /// Copier la valeur locale de la donn�e m_value vers le slider associ�
        m_interface->m_slider_value.set_value(m_value);

        /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
        m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
    }

}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if(m_actif == true)
    {
        if (!m_interface)
            return;

        /// Reprendre la valeur du slider dans la donn�e m_value locale
        m_value = m_interface->m_slider_value.get_value();
    }
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    /// Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    /// Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLEUCLAIR);

    /// Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  /// Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    m_top_edge.add_child( m_marche );
    m_marche.set_dim(40, 20);
    m_marche.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down );

    /// Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{


    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );

}

/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{

    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();

}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_legende_sauvegarde.set_message("SAVE ME");
    m_legende_sauvegarde.set_gravity_y(grman::GravityY::Down );
    m_legende_sauvegarde.set_gravity_x(grman::GravityX::Left );

    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);


    /// On ajoute une check-box m_connexit� pour lancer le programme connexit� en bas � droite
    m_connexite.set_dim(40, 20);
    m_connexite.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);



    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(FUCHSIACLAIR);

/// On ajoute une check-box m_animation en haut � droite
    //m_tool_box.add_child(m_animation);
    m_animation.set_dim(40, 20);
    m_animation.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    /// On ajoute une check-box sauvegarde en bas � gauche

    m_sauvegarde.set_dim(40, 20);
    m_sauvegarde.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down );

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);







}

///methode permettant � l'utilisateur de creer des graphes
int Graph::editer_reseaux(int choix1, int choix2)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);

    switch(choix1)
    {

    ///banquise
    case 1 :
    {
        m_vertices.clear();
        m_edges.clear();
        if(choix2==1)
        {

            char chaine[256];
            BITMAP*decor[12];
            for(int i =1; i<12; i++)
            {
                ///charger et afficher un sommet

                sprintf(chaine,"banquise/%d.jpg",i);

                add_interfaced_vertex(i, 30.0*i, 30*i, 30*i, chaine);


            }
            recup_arete("aretes_banquise.txt");
            remplissage_min_mout();
        }
        if(choix2==2)
        {
            std::string s1, s2;
            std::cout << " Quel fichier contenant vos sommets voulez-vous charger?" << std::endl << "Quel fichier d'aretes voulez-vous charger" << std::endl;
            std::cin >> s1;
            std::cin >> s2;
            recup_sommet(s1);

            recup_arete(s2);
            remplissage_min_mout();

        }
        break;

    }
    ///savane
    case 2 :
    {
        m_vertices.clear();
        m_edges.clear();

        if(choix2 ==1)
        {

            char chaine[256];
            BITMAP*decor[15];
            for(int i =1; i<15; i++)
            {
                ///charger et afficher un sommet
                sprintf(chaine,"savane/%d.jpg",i);
                add_interfaced_vertex(i, 30.0*i, 30*i, 30*i, chaine);


            }


            recup_arete("aretes_savane.txt");
            remplissage_min_mout();

        }

        if(choix2==2)
        {

            std::string s1, s2;
            std::cout << " Quel fichier contenant vos sommets voulez-vous charger?" << std::endl << "Quel fichier d'aretes voulez-vous charger" << std::endl;
            std::cin >> s1;
            std::cin >> s2;
            recup_sommet(s1);

            recup_arete(s2);
            remplissage_min_mout();

        }
        break;
    }

    ///foret
    case 3 :
    {
        m_vertices.clear();
        m_edges.clear();
        if(choix2==1)
        {
            char chaine[256];
            BITMAP*decor[15];
            for(int i =1; i<15; i++)
            {
                ///charger et afficher un sommet
                sprintf(chaine,"foret/%d.jpg",i);
                add_interfaced_vertex(i, 30.0*i, 30*i, 30*i, chaine);
            }


            recup_arete("aretes_foret.txt");
            remplissage_min_mout();
        }

        if(choix2==2)
        {

            std::string s1, s2;
            std::cout << " Quel fichier contenant vos sommets voulez-vous charger?" << std::endl << "Quel fichier d'aretes voulez-vous charger" << std::endl;
            std::cin >> s1;
            std::cin >> s2;
            recup_sommet(s1);

            recup_arete(s2);
            remplissage_min_mout();
        }
        break;
    }


    case 4 :
    {
        return 0;
        break;
    }
    }


}

///methode recup�ration des aretes du graphe dans le fichier
void Graph::recup_arete(std::string _nom)
{
    //Ouverture en lecture du fichier
    std::ifstream fichier(_nom, std::ios::in);
    //si le fichier est bien ouvert
    if(fichier)
    {
        //Variable pour stocker les mots du fichier
        std::string mot;

        //R�cup�ration du mot dans le fichier
        getline(fichier, mot);
        //Conversion de string vers int
        m_ordre =atoi(mot.c_str());

        std::cout << " ordre : " << m_ordre << std::endl << std::endl;

        //R�cup�ration du mot dans le fichier
        getline(fichier, mot);
        //Conversion de string vers int
        m_arete =atoi(mot.c_str());

        std::cout << " nb arete : " << m_arete << std::endl << std::endl;


        //on lit les aretes dans le fichier
        for(int i = 0; i< m_arete; i++)
        {

            int idx;
            int s1, s2;
            int poids =0;
            getline(fichier, mot, ' ') ;
            idx = atoi(mot.c_str());
            getline(fichier, mot, ' ') ;
            s1 = atoi(mot.c_str());
            getline(fichier, mot, ' ') ;
            s2 = atoi(mot.c_str());
            getline(fichier, mot, '\n') ;
            poids = atoi(mot.c_str());

            add_interfaced_edge(idx, s1, s2, poids);



        }

        for(auto elem : m_edges)
        {
            std::cout<<" poids de l'arete "<<elem.first<<" : "<<m_edges[elem.first].m_weight<<std::endl;
        }



        //fermeture du fichier
        fichier.close();
        for(auto elem : m_edges)
        {
            std::cout << elem.first << " " << elem.second.m_from << " " << elem.second.m_to << " " << elem.second.m_weight<< std::endl;
        }


        //Si erreur d'ouverture
    }
    else
        std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;

}

///remplissage pour chaque sommet des tab d'aretes sortante et entrantes
void Graph::remplissage_min_mout()
{

    ///on parcourt tous les sommets
    for(auto elem2 : m_vertices)
    {


        ///on parcours les aretes
        for(auto elem : m_edges)
        {
            ///si le 1er sommet de l'arete est le meme que notre sommet
            if((elem.second).m_from == elem2.first)
            {
                ///on ajoute l'indice de cette arete dans le vec d'aretes sortantes
                m_vertices[(elem2.first)].m_out.push_back(elem.first) ;



            }


            ///si le 2eme sommet de l'arete est le meme que notre sommet
            if((elem.second).m_to == elem2.first)
            {
                ///on ajoute l'indice de cette arete dans le vec d'aretes entrantes
                m_vertices[(elem2.first)].m_in.push_back(elem.first) ;


            }

        }
    }

}



void Graph::sauvegarde()
{
///donnees a sauvegarder :m_ordre, index/sommet + coord + N, map aretes
    std::string m_sauvegarde1, m_sauvegarde2;

    m_interface->m_sauvegarde.set_value(false);

    std::cout<<"entrer le nom du fichier dans lequel vous souhaitez sauvegarder les sommets du graphe svp "<<std::endl;
    std::cin>>m_sauvegarde1;
    std::fstream fichier(m_sauvegarde1, std::ios::out|std::ios::app);

    if(fichier)
    {
        fichier.clear();
        fichier << m_ordre<<'\n';
        for(auto elem : m_vertices)
        {
            fichier<< elem.first<<' '<<elem.second.m_interface->m_top_box.get_posx()<<' '<<elem.second.m_interface->m_top_box.get_posy()<<' '<<elem.second.m_value<<'\n';

        }

        fichier.close();
        std::cout<<" sauvegarde sommets effectuee"<<std::endl;

    }
    else
        std::cerr <<" impossible d'ouvrir ton fichier nigga "<<std::endl;

    std::cout<<"entrer le nom du fichier dans lequel vous souhaitez sauvegarder les sommets du graphe svp "<<std::endl;
    std::cin>>m_sauvegarde2;
    std::fstream fichier2(m_sauvegarde2, std::ios::out|std::ios::app);

    if(fichier2)
    {
        fichier2.clear();
        fichier2 << m_ordre<<'\n';
        fichier2 << m_arete<<'\n';
        for(auto elem : m_edges)
        {
            fichier2<< elem.first<<' '<<elem.second.m_from<<' '<<elem.second.m_to<<' '<<elem.second.m_weight<<'\n';

        }

        fichier2.close();
        std::cout<<" sauvegarde aretes effectuee"<<std::endl;

    }
    else
        std::cerr <<" impossible d'ouvrir ton fichier nigga "<<std::endl;


}

std::shared_ptr<GraphInterface> Graph::get_interface()
{
    return m_interface;
}


void Graph::supp_sommet_selec()
{
    ///On regarde l'�tat des checkbox de tous les sommet
    ///Si l'�tat est a true (on a cliqu� dessus pour le supprimer
    std::vector <int> indice_supp;
    for(auto elem : m_vertices)
    {


        if(m_vertices[elem.first].m_interface->m_marche.get_value() == true)
        {
            indice_supp.push_back(elem.first);

        }
    }

    for( int i = 0; i< indice_supp.size(); i++)
    {
        std::cout << "indice du sommet a supprimer : " << i << std::endl;
        supp_sommet(indice_supp[i]);
        ///Si il y a un affichage actif
        if (m_interface && m_vertices[indice_supp[i]].m_interface)

        {
            ///On le supprime � l'�cran
            m_interface->m_main_box.remove_child( m_vertices[indice_supp[i]].m_interface->m_top_box );

        }

        m_vertices.erase(indice_supp[i] );

    }



}
void Graph::supp_sommet(int vdx)
{
    std::cout << "dans supp_sommet" << std::endl;



    ///On supprime toutes les arretes contenu dans m_in

    for(int i= m_vertices[vdx].m_in.size()-1 ; i>=0; i--)
    {
        std::cout<<"i "<<i;
        ///On supprime l'arete dans la map m_edges ( suppression �galement dans min et mout)
        supp_arete(m_vertices[vdx].m_in[i]);


    }


    ///On supprime toutes les arretes contenu dans m_out
    for(int j= m_vertices[vdx].m_out.size()-1; j>=0; j--)
    {
        std::cout<<"j "<<j;
        supp_arete(m_vertices[vdx].m_out[j]);


    }

}


void Graph::supp_arete(int eidx)
{
    /// r�f�rence vers le Edge � enlever

    Edge &remed=m_edges.at(eidx);


    std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;





    /// test : on a bien des �l�ments interfac�s

    if (m_interface && remed.m_interface )

    {



        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );

    }



    /// Il reste encore � virer l'arc supprim� de la liste des entrants et sortants des 2 sommets to et from !

    /// References sur les listes de edges des sommets from et to

    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;

    std::vector<int> &veto = m_vertices[remed.m_to].m_in;

    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );

    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );



    /// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)

    /// Il suffit donc de supprimer l'entr�e de la map pour supprimer � la fois l'Edge et le EdgeInterface

    /// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !

    m_edges.erase( eidx );



    std::cout << " aretes actualis�es apr�s suppression " << std::endl;

}


void Graph::supp_arete_test()
{
    /// r�f�rence vers le Edge � enlever
    for(auto elem : m_edges)
    {

        if(m_edges[elem.first].m_interface->m_marche.get_value() == true)
        {
            Edge &remed=m_edges.at(elem.first);




            std::cout << "Removing edge " << elem.first << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;



            /* /// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2

             std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;

             std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;

             std::cout << m_edges.size() << std::endl;

            */

            /// test : on a bien des �l�ments interfac�s

            if (m_interface && remed.m_interface)

            {

                /// Ne pas oublier qu'on a fait �a � l'ajout de l'arc :

                /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */

                /* m_interface->m_main_box.add_child(ei->m_top_edge);  */

                /* m_edges[idx] = Edge(weight, ei); */

                /// Le new EdgeInterface ne n�cessite pas de delete car on a un shared_ptr

                /// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)

                /// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe

                m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );

            }



            /// Il reste encore � virer l'arc supprim� de la liste des entrants et sortants des 2 sommets to et from !

            /// References sur les listes de edges des sommets from et to

            std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;

            std::vector<int> &veto = m_vertices[remed.m_to].m_in;

            vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), elem.first ), vefrom.end() );

            veto.erase( std::remove( veto.begin(), veto.end(),elem.first ), veto.end() );



            /// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)

            /// Il suffit donc de supprimer l'entr�e de la map pour supprimer � la fois l'Edge et le EdgeInterface

            /// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !

            m_edges.erase( elem.first );






            /*  /// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2

              std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;

              std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;

              std::cout << m_edges.size() << std::endl;
            */
        }

    }
}

///methode recup sommet
void Graph::recup_sommet(std::string _nom)
{

    //Ouverture en lecture du fichier
    std::ifstream fichier(_nom, std::ios::in);
    //si le fichier est bien ouvert
    if(fichier)
    {
        //Variable pour stocker les mots du fichier
        std::string mot;

        //R�cup�ration du mot dans le fichier
        getline(fichier, mot);
        //Conversion de string vers int

        m_ordre =atoi(mot.c_str());
        char chaine[256];
        BITMAP*decor[m_ordre+1];
        std::cout << " ordre : " << m_ordre << std::endl << std::endl;

        //on lit les aretes dans le fichier
        for(int i = 1; i< m_ordre+1; i++)
        {
            sprintf(chaine,"banquise/%d.jpg",i);

            int idx;
            int posx, posy;
            int value = 0;

            getline(fichier, mot, ' ') ;
            idx = atoi(mot.c_str());
            getline(fichier, mot, ' ') ;
            posx = atoi(mot.c_str());
            getline(fichier, mot, ' ') ;
            posy= atoi(mot.c_str());
            getline(fichier, mot, '\n') ;
            value = atoi(mot.c_str());
            add_interfaced_vertex(idx, value, posx, posy, chaine);



        }





        //fermeture du fichier
        fichier.close();
        for(auto elem : m_vertices)
        {
            std::cout << elem.first << " " << elem.second.m_interface->m_top_box.get_posx() <<  " " << elem.second.m_interface->m_top_box.get_posy() << " " << elem.second.m_value << std::endl ;
        }


        //Si erreur d'ouverture
    }
    else
        std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;

}

///methode pour colorier un sommet
void Graph::colorier_som(Vertex v, char coul)
{

    ///on attribue une couleur au sommet actuel
    v.m_coul = coul;
    v.m_interface->m_top_box.set_bg_color(coul);

    ///Si le sommet v a des aretes sortantes
    if(v.m_out.size()!=0)
    {
        ///On parcours les aretes sortantes du sommet v
        for(int i=0; i<v.m_out.size(); i++)

        {   ///On parcours la map de sommet
            for(auto elem: m_vertices)
            {
                ///Si les sommets ont des aretes entrantes
                if(m_vertices[elem.first].m_in.size()!=0)
                {
                    ///On parcours les aretes entrantes de chaque sommet
                    for(int j =0; j<m_vertices[elem.first].m_in.size(); j++)
                    {
                        ///Si l'arete entrante du sommet est �gale � l'arete sortante du sommet v
                        if((elem.second).m_in[j]  == v.m_out[i])
                        {
                            ///Si le voisin n'est pas colorier et qu'il est actif
                            if(m_vertices[elem.first].m_coul == ' '  && m_vertices[elem.first].m_actif == true)
                            {
                                ///On le colorie
                                m_vertices[elem.first].m_coul = coul;
                                ///On rappel le sous programme
                                colorier_som(m_vertices[elem.first], coul);
                            }
                        }
                    }
                }

            }

        }

    }

    ///On parcours les aretes entrantes du sommet v
    if(v.m_in.size()!=0)
    {
        ///on parcourt les indices d'aretes entrantes de v
        for(int i=0; i<v.m_in.size(); i++)
        {
            ///on parcourt les sommets du graphe
            for(auto elem: m_vertices)
            {
                if(m_vertices[elem.first].m_out.size()!=0)
                {
                    ///on parcourt les aretes sortantes de v
                    for(int j =0; j<m_vertices[elem.first].m_out.size(); j++)
                    {
                        ///si l'arete sortantes du sommet est �gale � l'arete entrante de v
                        if((elem.second).m_out[j]  == v.m_in[i])
                        {
                            ///S'il n'a pas de couleurs et qu'il est actif
                            if(m_vertices[elem.first].m_coul == ' ' && m_vertices[elem.first].m_actif == true )
                            {
                                ///On le colorie
                                m_vertices[elem.first].m_coul = coul;
                                colorier_som(m_vertices[elem.first], coul);
                            }
                        }
                    }
                }

            }

        }

    }
}

///methode recherhce toutes les compo connexes
int Graph ::colorier_compo_connexe()
{
    m_interface->m_connexite.set_value(false);
    for(auto elem : m_vertices)
    {
        m_vertices[elem.first].m_interface->m_top_box.set_bg_color(BLANC);
        m_vertices[elem.first].m_coul = ' ';
    }
    ///variable de couleur
    int coul = JAUNE;

    ///compteur nb compo connexes
    int nb_compo = 0;

    ///on parcourt les sommets
    for(auto elem : m_vertices)
    {
        ///si le sommet n'est pas colori�
        if ((m_vertices[(elem.first)].m_coul) == ' ' && m_vertices[elem.first].m_actif == true)
        {


            ///on choisi une nouvelle couleur
            coul=COULEURALEATOIRE;

            ///on incr�mente le nb de compo connexe
            nb_compo++;

            ///on colorie le sommet avec la nouvelle couleur
            m_vertices[(elem.first)].m_coul = coul;

            ///on appelle la methode colorier sommet avec parametres le sommet et la new couleur
            colorier_som(elem.second, coul);
        }
    }
    std::cout << " nb composantes connexes: " << nb_compo << std::endl;
    for(auto elem : m_vertices)
    {
        m_vertices[elem.first].m_coul = ' ';
    }
    return nb_compo;
}

void Graph::remplir_matrice_adjacence()
{

    m_adjacence = new int*[m_ordre];
    for(int i= 0; i<m_ordre; i++)
    {
        m_adjacence[i] = new int[m_ordre];
    }


    for(int i = 0; i< m_ordre; i++)
    {
        for(int j= 0; j< m_ordre; j++)
        {
            m_adjacence[i][j] = 0;
        }
    }


    for(auto elem: m_edges)
    {


        m_adjacence[elem.second.m_from - 1][elem.second.m_to - 1] = 1;
    }


}

void Graph::fortement_connexe( )
{
    int** fort;

    fort = new int*[m_ordre];
    for(int i= 0; i<m_ordre; i++)
    {
        fort[i] = new int[m_ordre];
    }


    remplir_matrice_adjacence();
    fort = toutes_compo_forte_connexe();

    for(int i=0; i< m_ordre; i++)
    {
        for(int j=0; j<m_ordre; j++)
        {

                if(fort[i][j] != 0)
                {
            m_vertices[i+1].m_interface->m_top_box.set_bg_color(fort[i][j]);
            m_vertices[j+1].m_interface->m_top_box.set_bg_color(fort[i][j]);
                }

        }

    }

        graphe_reduit(fort);



}

void Graph::graphe_reduit(int** fort)
{
    std::map<int, std::vector<int> > compo;
    std::vector<int> compo_inter;
    int numero_compo = m_ordre+1;
        ///On cherche les membres de la composante connexe

                for(int i= 0; i< m_ordre; i++)
                {      numero_compo++;
                    for(int k = 0; k< compo_inter.size(); k++)
                    {
                        compo_inter.pop_back();
                    }
                    for(int j= 0; j<m_ordre; j++)
                    {
                        //std::cout << " couleur de  i " << i+1 << " est " <<m_vertices[i+1].m_interface->m_top_box.get_bg_color() << std::endl;
                        //std::cout << " couleur de j " << j+1 << " est " <<m_vertices[j+1].m_interface->m_top_box.get_bg_color() << std::endl;
                        if((m_vertices[i+1].m_interface->m_top_box.get_bg_color() == m_vertices[j+1].m_interface->m_top_box.get_bg_color()) && fort[i][j]!=0)
                        {
                            std::cout << "ajout de " << j+1 << "dans " << numero_compo <<std::endl;
                            //std::cout << "avant ajout vector " << std::endl;
                            compo_inter.push_back(j+1);

                        }

                    }
                        if(compo_inter.size()!=0)
                        {
                            compo.insert ( std::pair<int ,std::vector<int>>(numero_compo,compo_inter) );
                        }

                    std::cout << "taille vecteur " << compo_inter.size() << std::endl;
                    for(int k = 0; k< compo_inter.size(); k++)
                    {
                        compo_inter.pop_back();
                    }

                }



        for(auto elem : compo)
        {
            std::cout << "numero compo : " << elem.first << " valeurs : " ;
            for(int i = 0; i< elem.second.size(); i++)
            {
                std::cout << elem.second[i] <<" ";
            }
            std::cout << std::endl;
        }





        for(auto elem: compo)
        {std::vector<int> in;
        std::vector<int> out;



            for(int i= 0; i< elem.second.size(); i++)
            {



                    for(int i= 0; i< m_vertices[elem.second[i]].m_in.size(); i++)
                    {
                        in.push_back(m_vertices[elem.second[i]].m_in[i]);
                        //std::cout << "ajout dans m_in de " << m_vertices[elem.second[i]].m_in[i] << std::endl;
                    }
                    for(int i= 0; i< m_vertices[elem.second[i]].m_out.size(); i++)
                    {
                        out.push_back(m_vertices[elem.second[i]].m_out[i]);
                        //std::cout << "ajout dans m_out de " << m_vertices[elem.second[i]].m_out[i] << std::endl;

                    }

            }
            std::cout <<" sommet numero :" << elem.first << std::endl;
        std::cout << "m_in = " ;
        for(int i = 0; i<in.size(); i++)
        {
                std::cout << in[i] << " ";
        }
        std::cout << "m_out = " ;
        for(int i = 0; i<out.size(); i++)
        {
                std::cout << out[i] << " ";
        }
        /*for(int i = 0; i<in.size(); i++)
        {
                in.pop_back();
        }

        for(int i = 0; i<out.size(); i++)
        {
                out.pop_back();        }*/
            }

                    ///Appeler ajout sommet en lui passant m_in et m_to





}
int ** Graph::toutes_compo_forte_connexe()
{
    ///Variables locales
    int** tabc;///Tableau dynamique des composantes fortements connexes � retourner
    int* marques; ///Tableau de marquage
    int x, y; ///Numero des sommets interm�diares comme indice des tableaux interm�diairs
    int nb_fort_connexe = 0;
    int coul = BLANC;
    int chiffre = 0;
    ///variable de couleur

    ///Allocation des tableaux tabc et marques de taille ordre
    tabc = new int*[m_ordre];
    for(int i= 0; i<m_ordre; i++)
    {
        tabc[i] = new int[m_ordre];
    }
    marques = new int[m_ordre];



    ///Initialisation des valeurs � 0
    for(int i= 0; i<m_ordre; i++)
    {
        for(int j= 0; j< m_ordre; j++)
        {
            tabc[i][j] = 0;
        }

    }
    for(int j= 0; j< m_ordre; j++)
    {
        marques[j] = 0;
    }

    for(auto elem: m_vertices)
    {
        m_vertices[elem.first].m_interface->m_top_box.set_bg_color(BLANC);
    }
    ///Pour tous les sommets x non marqu�s
    ///Recherche compo fortement connexe de x
    ///Marquer chaque sommet x et marquer les sommets y connect�s a x non marqu�s
    for(x =0; x<m_ordre; x++)
    {
        if (!marques[x])
        {

            ///on choisi une nouvelle couleur

            coul = coul * 10;
            tabc[x] = Une_Compo_Forte_Connexe(x, coul);
            marques[x] = 1;
            nb_fort_connexe++;



            for(y=0; y<m_ordre; y++)
            {
                if(tabc[x][y] && !marques[y])
                {
                    marques[y]= 1;
                }
            }
        }
    }

    std::cout << " Nombre de composante fortement connexe : " << nb_fort_connexe << std::endl;
    return tabc;
}
///methode recherche toutes les compo fortement connexes
int* Graph::Une_Compo_Forte_Connexe(int s, int chiffre)
{

    int * c1, *c2; ///compo connexes directes partant de s et indirectes arrivant vers s
    int *c; ///Compo fortement connexe = intersection entre c1 et c2 ( � retourner)
    int * marques; ///tableau de marquages
    int x, y; ///num�ros de sommets interm�diaires des composantes connexes
    int ajoute = 1; /// bool indiquant si une nouvelle composante connexe esr ajout�e


    ///Allocation tableau c1 et c2 et c et marque de taille ordre

    marques = new int[m_ordre];
    c1 = new int[m_ordre];
    c2= new int[m_ordre];
    c = new int[m_ordre];



    ///Initialisation valeurs tab � 0;
    for(int i= 0; i<m_ordre; i++)
    {
        marques[i] = 0;
    }
    for(int i= 0; i<m_ordre; i++)
    {
        c1[i] = 0;
    }
    for(int i= 0; i<m_ordre; i++)
    {
        c2[i] = 0;
    }
    for(int i= 0; i<m_ordre; i++)
    {
        c[i] = 0;
    }

    ///Rendre le sommet s connexe
    c1[s] = chiffre;
    c2[s] = chiffre;

    ///Recherche des compo connexes partant de s � ajouter dans c1
    while(ajoute)
    {
        ajoute = 0; ///a chaque tour, recherche d'une nouvelle compo connexe a ajouter

        ///Pour tous les sommets x non marqu�s et connect�s en partant de s
        ///Marquer chaque sommet x et connecter les sommets non marqu�s y adjacents � x
        for(x = 0; x<m_ordre ; x++)
        {
            if(!marques[x] && c1[x])
            {
                marques[x] = 1;
                for(y = 0; y< m_ordre; y++)
                {
                    if(m_adjacence[x][y] && !marques[y])
                    {
                        c1[y] = chiffre;
                        ajoute = 1; ///Nouvelle composante connexe a ajouter
                    }
                }
            }
        }


    }

    for(int i= 0; i<m_ordre; i++)
    {
        marques[i] = 0;
    }

    ajoute = 1;
    ///On recherche les composantes connexes arrivant � s � ajouter dans c2
    while(ajoute)
    {

        ajoute = 0; ///a chaque tour, recherche d'une nouvelle compo connexe a ajouter

        ///Pour tous les sommets x non marqu�s et connect�s en partant de s
        ///Marquer chaque sommet x et connecter les sommets non marqu�s y adjacents � x
        for(x = 0; x<m_ordre ; x++)
        {

            if(!marques[x] && c2[x])
            {
                marques[x] = 1;

                for(y = 0; y< m_ordre; y++)
                {

                    if(m_adjacence[y][x] && !marques[y])
                    {

                        c2[y] = chiffre;
                        ajoute = 1; ///Nouvelle composante connexe a ajouter
                    }
                }
            }
        }


    }

    ///Composantes fortement connexe c = intersection c1 et c2
    for(x = 0; x<m_ordre; x++)
    {
        c[x] = c1[x] & c2[x];
    }
    ///Retourner la composante fortement connexe c
    return c;
}

///methode gestion dynamique population 1
void Graph::dynamique_pop1()
{
    /* int sommet; ///= indice sommet choisi
     int r=2; ///rythme de croissance
     int k=100; ///capacit� de portage de l'environnement
     int n; ///valeur (choisie par l'utilisateur) attribu�e  au sommet
     */

    while (!key[KEY_UP])//m_interface->m_animation.get_value() == true
    {

        std::cout << "dans le while" << std::endl;
        ///a chaque tour de boucle la population croit ou decroit
        for(auto elem : m_vertices)
        {
            std::cout << "dans le for" << std::endl;
            ///evolution en fonction du temps
            dynamique_pop2(elem.first);
            update();
            /*m_vertices[elem.first].m_value = m_vertices[elem.first].m_value + r*m_vertices[elem.first].m_value*(1 - m_vertices[elem.first].m_value/k);
             std::cout<<" m_value " <<m_vertices[elem.first].m_value<<std::endl;
             m_vertices[elem.first].m_interface->m_slider_value.set_range(0.0, m_vertices[elem.first].m_value);
            */
        }
    }


}

///methode gestion dynamique population 2

void Graph::dynamique_pop2(int sommet)
{
    std::cout << "dans le sous prog" << std::endl;

    for(auto elem: m_vertices)
    {
        m_vertices[elem.first].m_value = m_vertices[elem.first].m_interface->m_slider_value.get_value();
        std::cout << elem.first << " value : " <<elem.second.m_value << std::endl;
    }


    ///= indice sommet choisi
    int r=30; ///rythme de croissance
    int k=0; ///capacit� de portage de l'environnement
    int n; ///valeur (choisie par l'utilisateur) attribu�e  au sommet
    int c=0; ///valeur a soustraire
    ///on parcourt tous les sommets
    ///si on clique sur un


    ///on parcourt les aretes sortantes =  vecteur : m_vertices[sommet].m_out

    if(m_vertices[sommet].m_in.size()!= 0)
    {


        for(int i =0; i<m_vertices[sommet].m_in.size(); i++)
        {
///calcul de k

            k = k + m_edges[m_vertices[sommet].m_in[i]].m_weight * m_vertices[m_edges[m_vertices[sommet].m_in[i]].m_from].m_value;
            std::cout <<"dans le calcul de k, k = " << k << std::endl;
        }

        std::cout<<" k = "<<k<<std::endl;


        for(int i =0; i<m_vertices[sommet].m_out.size(); i++)
        {
            c = c + m_edges[m_vertices[sommet].m_out[i]].m_weight * m_vertices[m_edges[m_vertices[sommet].m_in[i]].m_to].m_value;
            std::cout <<"dans le calcul de c, c = " << c << std::endl;
        }

        ///on parcourt les aretes entrantes =  vecteur : m_vertices[sommet].m_in
        /*for(int i =0; i<m_vertices[sommet].m_out.size(); i++)
        {
            ///sommet voisin = vecteur :m_edges[m_vertices[sommet].m_in[i]].m_from
            std::cout<<" sommet voisin "<<m_edges[m_vertices[sommet].m_out[i]].m_to<<std::endl;


            ///valeur du sommet voisin = m_vertices[m_edges[m_vertices[sommet].m_in[i]].m_from].m_value
            n=m_vertices[m_edges[m_vertices[sommet].m_out[i]].m_to].m_value;
            std::cout << " n : " << n << std::endl;

              std::cout << " r*n*(1 - n/k) = " << r*n*(1 - n/k) << std::endl;
            n = n +r*n*(1 - n/k) - c;

            ///suppression des cas de populations n�gatives (impossible physiquement) = on minore � z�ro
            if (n <0)
            {
                n =0;
            }


            std::cout<<" n in= "<<n<<std::endl;


            ///On recupere la valeur de n dans l'attribut m_value du sommet point�
            m_vertices[m_edges[m_vertices[sommet].m_out[i]].m_to].m_value = n;
            std::cout << " apres ajout dans attribut " << std::endl;
            ///on maj la valeur du slider de notre sommet voisin
             m_vertices[m_edges[m_vertices[sommet].m_out[i]].m_to].m_interface->m_slider_value.set_value(n);
             std::cout << " maj interface " << std::endl;

        }*/



        ///on parcourt les aretes entrantes =  vecteur : m_vertices[sommet].m_in
        for(int i =0; i<m_vertices[sommet].m_in.size(); i++)
        {
            ///sommet voisin = vecteur :m_edges[m_vertices[sommet].m_in[i]].m_from
            std::cout<<" sommet voisin "<<m_edges[m_vertices[sommet].m_in[i]].m_from<<std::endl;


            ///valeur du sommet voisin = m_vertices[m_edges[m_vertices[sommet].m_in[i]].m_from].m_value
            n=m_vertices[m_edges[m_vertices[sommet].m_in[i]].m_from].m_value;
            std::cout << " n : " << n << std::endl;

            std::cout << " r*n*(1 - n/k) = " << r*n*(1 - n/k) << std::endl;
            n = n +r*n*(1 - n/k) - c;

            ///suppression des cas de populations n�gatives (impossible physiquement) = on minore � z�ro
            if (n <0)
            {
                n =0;
            }


            std::cout<<" n in= "<<n<<std::endl;


            ///On recupere la valeur de n dans l'attribut m_value du sommet point�
            m_vertices[m_edges[m_vertices[sommet].m_in[i]].m_from].m_value = n;
            std::cout << " apres ajout dans attribut " << std::endl;
            ///on maj la valeur du slider de notre sommet voisin
            m_vertices[m_edges[m_vertices[sommet].m_in[i]].m_from].m_interface->m_slider_value.set_value(n);
            std::cout << " maj interface " << std::endl;
            update();
        }
    }

}


void Graph::changement_nb_desactive()
{

    for(auto elem : m_vertices)
    {
        m_vertices[elem.first].m_actif = true;
        m_vertices[elem.first].m_coul = ' ';

        m_vertices[elem.first].m_interface->m_top_box.set_bg_color(BLANC);

    }
    int connexe = 0;
    bool trouve = false;
    std::vector<int> deco;
    m_k_min = 0;
    for( int k=1; k<m_ordre+1; k++)
    {
        if(k == 1 && m_k_min == 0)
        {
            std::cout << "k : " << k << std::endl;





            ///On retire k par k les sommets
            for(auto elem: m_vertices)
            {


                ///on d�sactive le sommet
                m_vertices[elem.first].m_actif = false;
                connexe = colorier_compo_connexe();
                m_vertices[elem.first].m_actif = true;


                if(connexe > 1)
                {
                    m_k_min = k;

                    trouve = true;
                    deco.push_back(elem.first);

                }
            }


            std::cout << "k_min :" << m_k_min << std::endl;
            for(int i = 0; i< deco.size(); i++)
            {
                std::cout << "sommet a retirer " << deco[i] << std::endl;

                    m_vertices[deco[i]].m_interface->m_top_box.set_bg_color(ROUGE);

            }
            for(int i = 0; i< deco.size(); i++)
            {
                deco.pop_back();
            }
        }




        if(k != 1  )
        {





            while(m_k_min == 0)
            {
                for (int i = 0; i < m_ordre; ++i)
                {
                    m_combinaison_partielle.push_back(i+1);

                }


                boucle_combinaison( 0, k, k);





            for (int i = 0; i < m_combinaison_partielle.size(); ++i)
    {
        m_combinaison_partielle.pop_back();

    }
    for (int i = 0; i < m_combinaison_totale.size(); ++i)
    {
        m_combinaison_totale.pop_back();

    }

}


        }



    }
    m_k_min = 0;
    m_trouve = false;

    std::cout << "fin calcul k_connexite" << std::endl;
    return;
}


void Graph::boucle_combinaison(int offset, int numero_de_k_uplets, int k)
{



    if(numero_de_k_uplets == 0 )
    {

        k_connexite(k);

        return ;
    }
    else
    {

        for(int i=0; i< m_combinaison_partielle.size() - (numero_de_k_uplets); i++)
        {

            m_combinaison_totale.push_back(m_combinaison_partielle[i]);
            boucle_combinaison(i+1, numero_de_k_uplets -1, k);
            m_combinaison_totale.pop_back();

        }
    }


}


///M�thode  k-connexit�
void Graph::k_connexite(int k)
{


    ///BUT : Trouver combien au minimum de sommet doit-on enlever pour deconnecter le graphe et lesquels
    int connexe = 0;
    ///nb min pour deco graphe
    std::vector<int> deco;


    for(int i=0; i< m_combinaison_totale.size(); i++)
    {

        ///on d�sactive le sommet

        m_vertices[m_combinaison_totale[i]].m_actif = false;
        std::cout << " elem a false : " << m_combinaison_totale[i] << std::endl;

    }

    for(auto elem: m_vertices)
    {
        std::cout << elem.first << " :" << m_vertices[elem.first].m_actif << std::endl;
    }

    connexe = colorier_compo_connexe();



    for(int i=0; i< m_combinaison_totale.size(); i++)
    {
        m_vertices[m_combinaison_totale[i]].m_actif = true;


        if(connexe > 1)
        {
            m_trouve = true;

            m_k_min = k;
            deco.push_back( m_combinaison_totale[i]);



        }
    }

    for(auto elem: m_vertices)
    {
        std::cout << elem.first << " :" << m_vertices[elem.first].m_actif << std::endl;
    }

    std::cout << "k_min :" << m_k_min << std::endl;

    for(int i = 0; i< deco.size(); i++)
    {
        std::cout << "sommet a retirer " << deco[i] << std::endl;

        m_vertices[deco[i]].m_interface->m_top_box.set_bg_color(ROUGE);

    }

    for(int i=0; i<deco.size(); i++)
    {
        deco.pop_back();
    }


}





/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    m_interface->m_connexite.update();
    m_interface->m_tool_box.update();
    m_interface->m_sauvegarde.update();
    m_interface->m_animation.update();
    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();
    // m_interface->m_main_box.update();
    //m_interface->m_sauvegarde.update_pre_draw();
    //m_interface->m_sauvegarde.interact_focus();
    //m_interface->m_sauvegarde.update_draw();

    m_interface->m_tool_box.update();
    m_interface->m_sauvegarde.update();
    m_interface->m_connexite.update();
    m_interface->m_animation.update();
    m_interface->m_legende_sauvegarde.update();


    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

    m_interface->m_tool_box.update();
    m_interface->m_sauvegarde.update();
    m_interface->m_animation.update();
    m_interface->m_connexite.update();
}

/// Aide � l'ajout de sommets interfac�s
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);


    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);

}

/// Aide � l'ajout d'arcs interfac�s
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei, id_vert1, id_vert2);
}

