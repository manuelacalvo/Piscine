#include "graph.h"
#include <fstream>
#include <cstdlib>

/***************************************************
                    VERTEX
****************************************************/

/// Constructeur permettant la mise en place des éléments graphique de chaque sommet
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{

    /// On ajoute une check-box en bas à gauche
    m_top_box.add_child( m_marche );
    m_marche.set_dim(20, 10);
    m_marche.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up );


    /// On ajoute en haut à gauche du cadre un sous-cadre (une boite dans la boite)

    // La boite englobante du sommet
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(100, 70);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur pour chaque sommet
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 200);
    m_slider_value.set_dim(10,40);
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


/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if(m_actif == true)
    {
        if (!m_interface)
            return;

        /// Copier la valeur locale de la donnée m_value vers le slider associé
        m_interface->m_slider_value.set_value(m_value);

        /// Copier la valeur locale de la donnée m_value vers le label sous le slider
        m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
    }


}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if(m_actif == true)
    {
        if (!m_interface)
            return;

        /// Reprendre la valeur du slider dans la donnée m_value locale
        m_value = m_interface->m_slider_value.get_value();
    }

}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les éléments de l'interface
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

    /// Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLEUCLAIR);

    /// Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 200.0);
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    m_top_edge.add_child( m_marche );
    m_marche.set_dim(20, 10);
    m_marche.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down );

    /// Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{


    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );

}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{

    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();

}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)

{
    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(FUCHSIACLAIR);

    /// On ajoute une check-box sauvegarde en bas à gauche
    m_tool_box.add_child(m_sauvegarde);
    m_legende_sauvegarde.set_message("SAUVER");
    m_sauvegarde.add_child(m_legende_sauvegarde);
    m_sauvegarde.set_frame(0,500,80,20);
    m_sauvegarde.set_bg_color(FUCHSIA);
    m_legende_sauvegarde.set_gravity_y(grman::GravityY::Down );
    m_legende_sauvegarde.set_gravity_x(grman::GravityX::Center );

    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);


    /// On ajoute une check-box m_connexité pour lancer le programme connexité en bas à droite
    m_connexite.set_dim(80, 20);
    m_connexite_text.set_message("CONNEXITE");
    m_connexite.set_bg_color(BLEUCLAIR);
    m_connexite.set_frame(0,230,80,20);
    m_tool_box.add_child(m_connexite);
    m_connexite.add_child(m_connexite_text);
    m_connexite_text.set_gravity_x(grman::GravityX::Center );


    m_legende_fort_connexite.set_message("FCONNEXITE");
    m_fort_connexite.set_bg_color(BLEUCLAIR);
    m_fort_connexite.set_frame(0,250,80,20);
    m_tool_box.add_child(m_fort_connexite);
    m_fort_connexite.add_child(m_legende_fort_connexite);
    m_legende_fort_connexite.set_gravity_x(grman::GravityX::Center );

    m_legende_k_connexite.set_message("KCONNEXITE");
    m_k_connexite.set_bg_color(BLEUCLAIR);
    m_k_connexite.set_frame(0,290,80,20);
    m_tool_box.add_child(m_k_connexite);
    m_k_connexite.add_child(m_legende_k_connexite);
    m_legende_k_connexite.set_gravity_x(grman::GravityX::Center );

    m_legende_graph_reduit.set_message("REDUIT");
    m_graph_reduit.set_bg_color(JAUNESOMBRE);
    m_graph_reduit.set_frame(0,270,80,20);
    m_tool_box.add_child(m_graph_reduit);
    m_graph_reduit.add_child(m_legende_graph_reduit);
    m_legende_graph_reduit.set_gravity_x(grman::GravityX::Center );

    m_legende_quitter.set_message("QUITTER");
    m_quitter.set_bg_color(VIOLETCLAIR);
    m_quitter.set_frame(0,550,80,20);
    m_tool_box.add_child(m_quitter);
    m_quitter.add_child(m_legende_quitter);
    m_legende_quitter.set_gravity_x(grman::GravityX::Center );



/// On ajoute une check-box m_animation en haut à droite
    m_tool_box.add_child(m_animation1);
    m_animation1.set_bg_color(FUCHSIA);
    m_animation1.set_frame(3,3,80,20);
    m_animation1.add_child(m_legende_animation1);
    m_legende_animation1.set_message("SIMULATION1");
    m_legende_animation1.set_gravity_x(grman::GravityX::Center );

    m_tool_box.add_child(m_animation2);
    m_animation2.set_bg_color(FUCHSIA);
    m_animation2.set_frame(3,25,80,20);
    m_animation2.add_child(m_legende_animation2);
    m_legende_animation2.set_message("SIMULATION2");
    m_legende_animation2.set_gravity_x(grman::GravityX::Center );

    m_top_box.add_child(m_menu);
    m_menu.set_bg_color(VERTCLAIR);
    m_menu.set_frame(350,10,90,30);
    m_menu.add_child(m_legende_menu);
    m_legende_menu.set_message("MENU");
    m_legende_menu.set_gravity_x(grman::GravityX::Center );




    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);


    ///  un 1er bouton ajout sommet
    m_tool_box.add_child( m_ajout_s );
    m_ajout_s.set_frame(0,150,80,20);
    m_ajout_s.set_bg_color(FUCHSIA);
    m_ajout_s.add_child(m_ajout_s_text);
    m_ajout_s_text.set_message("AJOUT_S");

    ///  un 1er bouton ajout arete
    m_tool_box.add_child( m_ajout_a );
    m_ajout_a.set_frame(0,130,80,20);
    m_ajout_a.set_bg_color(FUCHSIA);
    m_ajout_a.add_child(m_ajout_a_text);
    m_ajout_a_text.set_message("AJOUT_A");



}

///methode permettant à l'utilisateur de creer des graphes
int Graph::editer_reseaux( int choix2)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);

    switch(m_monde)
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

                add_interfaced_vertex(i, 50, 30*i, 30*i, 1,chaine);


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
                add_interfaced_vertex(i, 50, 30*i, 30*i, 1, chaine);


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
                add_interfaced_vertex(i, 50, 30*i, 30*i, 1, chaine);
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

///methode recupération des aretes du graphe dans le fichier
void Graph::recup_arete(std::string _nom)
{
    //Ouverture en lecture du fichier
    std::ifstream fichier(_nom, std::ios::in);
    //si le fichier est bien ouvert
    if(fichier)
    {
        //Variable pour stocker les mots du fichier
        std::string mot;

        //Récupération du mot dans le fichier
        getline(fichier, mot);
        //Conversion de string vers int
        m_ordre =atoi(mot.c_str());

        std::cout << " ordre : " << m_ordre << std::endl << std::endl;

        //Récupération du mot dans le fichier
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

            add_interfaced_edge(idx, s1, s2, 1, poids);



        }




        //fermeture du fichier
        fichier.close();
        std::cout << "tableau d'arete du graphe" << std::endl;
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


///methode sauvegarde graphe
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

void Graph::supp_sommet_main()
{
    supp_sommet_selec(m_vertices, m_edges);
}

///methode supprimé sommet selec
void Graph::supp_sommet_selec(std::map<int, Vertex> _vertices, std::map<int,Edge> _edges)
{
    ///On regarde l'état des checkbox de tous les sommet
    ///Si l'état est a true (on a cliqué dessus pour le supprimer)
    std::vector <int> indice_supp;
    for(auto elem : _vertices)
    {


        if(_vertices[elem.first].m_interface->m_marche.get_value() == true)
        {

            indice_supp.push_back(elem.first);

        }
    }
    for( int i = 0; i< indice_supp.size(); i++)
    {

        supp_sommet(indice_supp[i], _vertices, _edges);
        ///Si il y a un affichage actif
        if (m_interface && _vertices[indice_supp[i]].m_interface)

        {
            ///On le supprime à l'écran
            m_interface->m_main_box.remove_child( _vertices[indice_supp[i]].m_interface->m_top_box );

        }



    }



}

///methode supprimé sommet
void Graph::supp_sommet(int vdx, std::map<int, Vertex> _vertices, std::map<int, Edge> _edges)
{



    ///On supprime toutes les arretes contenu dans m_in

    for(int i= _vertices[vdx].m_in.size()-1 ; i>=0; i--)
    {

        ///On supprime l'arete dans la map m_edges ( suppression également dans min et mout)
        supp_arete(_vertices[vdx].m_in[i], _edges, _vertices);


    }


    ///On supprime toutes les arretes contenu dans m_out
    for(int j= _vertices[vdx].m_out.size()-1; j>=0; j--)
    {

        supp_arete(_vertices[vdx].m_out[j], _edges, _vertices);


    }

}

///methode supprimé arete
void Graph::supp_arete(int eidx, std::map <int, Edge> _edges, std::map<int, Vertex> _vertices)
{
    /// référence vers le Edge à enlever
    Edge &remed=_edges.at(eidx);


    if (m_interface && remed.m_interface )

    {///on supprime l'interface des éléments
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );

    }



    ///suppression de la liste des entrants et sortants des 2 sommets to et from
    std::vector<int> &vefrom = _vertices[remed.m_from].m_out;

    std::vector<int> &veto = _vertices[remed.m_to].m_in;

    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );

    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );

    ///On supprime le sommet

    _edges.erase( eidx );


}

///methode supprimer arete
void Graph::supp_arete_main()
{
    /// référence vers le Edge à enlever
    for(auto elem : m_edges)
    {

        if(m_edges[elem.first].m_interface->m_marche.get_value() == true)
        {
            Edge &remed=m_edges.at(elem.first);


            /// test : on a bien des éléments interfacés

            if (m_interface && remed.m_interface)

            {



                m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );

            }



            ///Suppression de la liste des entrants et sortants des 2 sommets to et from
            std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;

            std::vector<int> &veto = m_vertices[remed.m_to].m_in;

            vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), elem.first ), vefrom.end() );

            veto.erase( std::remove( veto.begin(), veto.end(),elem.first ), veto.end() );


            ///Suppression arete
            m_edges.erase( elem.first );

        }

    }
}

///Méthode pour ajouter une arete choisi
void Graph::ajout_arete_selec()
{
    m_interface->m_ajout_a.set_value(false);
    int s1,s2;
    std::cout<<" choisissez les sommets à relier svp "<<std::endl;
    std::cin>>s1;
    std::cin>>s2;
    ///Appel au sous-programme ajout arete
    ajout_arete(s1,s2);
}

///methode ajout arete
void Graph::ajout_arete(int s_ajout, int s_relie)
{
    ///variables
    ///on recupere le sens de l'arete
    int sens;
    ///on recupere le poids de l'arete
    int poids;
    std::cout<<" choissisez le poids de l'arete "<<std::endl;
    std::cin>>poids;
    std::cout<<" 1.arete entrante  2. sortante pour votre sommet ajoute "<<std::endl;
    std::cin>>sens;
    int indice;
    ///on cherche ou ranger l'arete dans la map
    indice = m_edges.size()+1;
    m_arete++;

    ///cas d'une arete entrante
    if(sens ==1)
    {
        ///on cree une arete
        Edge e (poids,s_relie, s_ajout );
        ///on l'affiche
        add_interfaced_edge(indice, s_relie, s_ajout,1,  poids);
        ///on l'ajoute à la map d'arete

        m_edges.insert ( std::pair<int,Edge>(indice,e) );
        ///on remplie ses m_to et m_from
        m_edges[indice].m_from = s_relie;
        m_edges[indice].m_to = s_ajout;
    }



    ///cas d'une arete sortante
    if(sens ==2)
    {
        ///on cree une arete
        Edge e(poids, s_ajout, s_relie);
        ///on l'affiche
        add_interfaced_edge(indice, s_ajout, s_relie,1,   poids);
        ///on l'ajoute a la map d'arete
        m_edges.insert ( std::pair<int,Edge>(indice,e) );
        ///on remplie ses m_to et m_from
        m_edges[indice].m_to = s_relie;
        m_edges[indice].m_from = s_ajout;

    }


    ///on parcours les aretes
    for(auto elem : m_edges)
    {
        ///si le 1er sommet de l'arete est le meme que notre sommet
        if((elem.second).m_from == s_ajout)
        {
            ///on ajoute l'indice de cette arete dans le vec d'aretes sortantes
            m_vertices[s_ajout].m_out.push_back(elem.first) ;



        }


        ///si le 2eme sommet de l'arete est le meme que notre sommet
        if((elem.second).m_to == s_ajout)
        {
            ///on ajoute l'indice de cette arete dans le vec d'aretes entrantes
            m_vertices[s_ajout].m_in.push_back(elem.first) ;


        }

        ///si le 1er sommet de l'arete est le meme que le sommet a relier
        if((elem.second).m_from == s_relie)
        {
            ///on ajoute l'indice de cette arete dans le vec d'aretes sortantes
            m_vertices[s_relie].m_out.push_back(elem.first) ;



        }


        ///si le 2eme sommet de l'arete est le meme que le sommet à relier
        if((elem.second).m_to == s_relie)
        {
            ///on ajoute l'indice de cette arete dans le vec d'aretes entrantes
            m_vertices[s_relie].m_in.push_back(elem.first) ;


        }

        ///on ajouter les m_to et m_from de chaque arete


    }
}

///choix photo
void Graph::choix_photo( int s_ajout)
{
    char chaine[256];
    int photo;
    std::cout<<" choississez votre animal svp "<<std::endl;
    switch(m_monde)
    {
    case 1 :
    {
        std::cout<<"1.Baleine a bosse"<<std::endl<<"2.Elephant de mer"<<std::endl<<"3.Gorfou"<<std::endl<<"4.Gros poissons"<<std::endl<<"5.Le krill"<<std::endl<<"6.Leopard de mer"<<std::endl<<"7.Manchot Adelie"<<std::endl<<"8.Manchot empereur"<<std::endl<<"9.Orque"<<std::endl<<"10.Petits poissons"<<std::endl<<"11.Phoque"<<std::endl;
        std::cin>>photo;
        sprintf(chaine,"banquise/%d.jpg",photo);
        add_interfaced_vertex(s_ajout, 50, 100, 100, 1, chaine);
        break;
    }
    case 2 :
    {
        std::cout<<"1.Babouin"<<std::endl<<"2.Buffle"<<std::endl<<"3.Elephant"<<std::endl<<"4.Fourmi"<<std::endl<<"5.Gazelle"<<std::endl<<"6.Gnou"<<std::endl<<"7.Guepard"<<std::endl<<"8.Hippopotame"<<std::endl<<"9.Hyène"<<std::endl<<"10.Leopard"<<std::endl<<"11.Lion"<<std::endl<<"12.Phacochere"<<std::endl<<"13.Vegetaux"<<std::endl<<"14.Zebre"<<std::endl;
        std::cin>>photo;
        sprintf(chaine,"savane/%d.jpg",photo);
        add_interfaced_vertex(s_ajout, 50, 100, 100, 1, chaine);

        break;
    }
    case 3 :
    {
        std::cout<<"1.Aigle"<<std::endl<<"2.Araignee"<<std::endl<<"3.Arbre"<<std::endl<<"4.Biche"<<std::endl<<"5.Campagnol"<<std::endl<<"6.Chenille"<<std::endl<<"7.Hibou"<<std::endl<<"8.Criquet"<<std::endl<<"9.Ecureuil"<<std::endl<<"10.Vegetaux"<<std::endl<<"11.Lapin"<<std::endl<<"12.Pic vert"<<std::endl<<"13.Renard"<<std::endl<<"14.Sauterelle"<<std::endl;
        std::cin>>photo;
        sprintf(chaine,"foret/%d.jpg",photo);
        add_interfaced_vertex(s_ajout, 50, 100, 100, 1,  chaine);
        break;
    }
    }

}
///metgode ajouter sommet
void Graph::ajout_sommet_utilisateur()
{
    ///on remet le bouton à false (pas cliqué)
    m_interface->m_ajout_s.set_value(false);

    ///creation arete ou non
    int liaison =1;
    ///sommet a relié
    int s_relie;
    ///indice du sommet
    int indice;
    ///on recherche l'indice dans la map pour l'ajouter au bon endroit
    indice = m_vertices.size()+1;
    std::cout<<" Vous allez creer le sommet numero "<<indice;


    ///on lui demande l'animal (photo) qu'il souhaite ajouter
    choix_photo(indice);


    ///tant qu'il souhaite crée des aretes
    while(liaison !=2)
    {
        std::cout<<"1.creer une liaison  2. Stop "<<std::endl;
        std::cin>> liaison;

        if (liaison ==1)
        {
            ///on demande a l'utilisateur le sommet auquel il souhaite relier son sommet
            std::cout<<" a quel sommet souhaitez vous le relier "<<std::endl;
            std::cin>> s_relie;
            ///on appelle le ss prg d'ajout d'arete
            ajout_arete(indice,s_relie);

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

        //Récupération du mot dans le fichier
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
            add_interfaced_vertex(idx, value, posx, posy, 1, chaine);



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

    ///On parcours les aretes sortantes du sommet v
    if(v.m_out.size()!=0)
    {
        for(int i=0; i<v.m_out.size(); i++)
        {
            for(auto elem: m_vertices)
            {
                if(m_vertices[elem.first].m_in.size()!=0)
                {
                    for(int j =0; j<m_vertices[elem.first].m_in.size(); j++)
                    {

                        if((elem.second).m_in[j]  == v.m_out[i])
                        {
                            if(m_vertices[elem.first].m_coul == ' '  && m_vertices[elem.first].m_actif == true)
                            {
                                m_vertices[elem.first].m_coul = coul;
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
                        ///si l'indice
                        if((elem.second).m_out[j]  == v.m_in[i])
                        {
                            if(m_vertices[elem.first].m_coul == ' ' && m_vertices[elem.first].m_actif == true )
                            {
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

    }
    ///variable de couleur
    int coul = 'A';

    ///compteur nb compo connexes
    int nb_compo = 0;

    ///on parcourt les sommets
    for(auto elem : m_vertices)
    {
        ///si le sommet n'est pas colorié
        if ((m_vertices[(elem.first)].m_coul) == ' ' && m_vertices[elem.first].m_actif == true)
        {

            ///on choisi une nouvelle couleur
            coul ++;


            ///on incrémente le nb de compo connexe
            nb_compo++;

            ///on colorie le sommet avec la nouvelle couleur
            m_vertices[(elem.first)].m_coul = coul;

            ///on appelle la methode colorier sommet avec parametres le sommet et la new couleur
            colorier_som(elem.second, coul);
        }
    }
    for(auto elem :m_vertices)
    {
        int couleur = COULEURALEATOIRE;
        for(auto elem2 : m_vertices)
        {
            if(m_vertices[elem.first]. m_coul == m_vertices[elem2.first].m_coul)
            {
                m_vertices[elem.first].m_interface->m_top_box.set_bg_color(couleur);
                m_vertices[elem2.first].m_interface->m_top_box.set_bg_color(couleur);

            }
        }

    }
    std::cout << " nb composantes connexes: " << nb_compo << std::endl;
    for(auto elem : m_vertices)
    {
        m_vertices[elem.first].m_coul = ' ';
        m_vertices[elem.first].m_actif = true;
    }
    return nb_compo;
}



///methode etude connexité
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

///Appel sous-prog graphe réduit
void Graph::vider_graphe_reduit()
{

    ///On parcourt la map des compo fortements connexe
    for(auto elem: m_vertices_fortement_connexe)
    {


        if (m_interface && m_vertices_fortement_connexe[elem.first].m_interface)

        {
            ///On le supprime à l'écran
            m_interface->m_main_box.remove_child( m_vertices_fortement_connexe[elem.first].m_interface->m_top_box );

        }

    }

    for(auto elem: m_edges_fortement_connexe)
    {
        if (m_interface && m_edges_fortement_connexe[elem.first].m_interface)

        {
            ///On le supprime à l'écran
            m_interface->m_main_box.remove_child( m_edges_fortement_connexe[elem.first].m_interface->m_top_edge);

        }
    }


    ///on supprime les deux matrices
    m_vertices_fortement_connexe.clear();
    m_edges_fortement_connexe.clear();

}

///Méthode fortement_connexe
void Graph::fortement_connexe( )
{
    ///Tableau deux dimensions pour stocker la matrice d'adjacence fortement connexe
    int** fort;

    ///initialisation du tableau
    fort = new int*[m_ordre];
    for(int i= 0; i<m_ordre; i++)
    {
        fort[i] = new int[m_ordre];
    }


    ///On appel remplir matrice _adjacence liaison entre sommets
    remplir_matrice_adjacence();
    ///on rempli matrice adjacence compo
    fort = toutes_compo_forte_connexe();

    ///On les colorit de la meme couleur si ils font partis de la meme compo
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


    m_interface->m_graph_reduit.set_value(false);
    graphe_reduit(fort);



}

void Graph::graphe_reduit(int** fort)
{
    ///Map contenant le numero de la composante aini que les sommets la composants
    std::map<int, std::vector<int> > compo;
    ///Map stockant les sommets des composantes
    std::vector<int> compo_inter;
    ///Indice de la composante
    int numero_compo = 0;



    ///On cherche les sommets de la composante connexe
    ///On parcourt les lignes de notre matrice de composantes connexe
    for(int i= 0; i< m_ordre; i++)
    {
        ///On augmente le numero de la composante
        numero_compo++;
        ///On remet à zéro compo inter
        for(int k = 0; k< compo_inter.size(); k++)
        {
            compo_inter.pop_back();
        }

        ///On parcourt les colonnes de notre matrice de composantes connexe
        for(int j= 0; j<m_ordre; j++)
        {
            ///Si les deux sommets ont la même couleur, donc qu'ils font parti de la même composante fortement connexe
            if((m_vertices[i+1].m_interface->m_top_box.get_bg_color() == m_vertices[j+1].m_interface->m_top_box.get_bg_color()) && fort[i][j]!=0)
            {
                ///On les ajoute dans compo_inter
                compo_inter.push_back(j+1);

            }

        }
        ///s'il y a des sommets dans compo_inter
        if(compo_inter.size()!=0)
        {
            ///On récupère le numéro de la composante fortement connexe et ses sommets et on les ajoute dans compo
            compo.insert ( std::pair<int,std::vector<int>>(numero_compo,compo_inter) );
        }

        ///On remet à zéro
        for(int k = 0; k< compo_inter.size(); k++)
        {
            compo_inter.pop_back();
        }

    }


    ///Affichage des sommets constituants les composantes fortements connexes
    for(auto elem : compo)
    {
        std::cout << "numero compo : " << elem.first << " valeurs : " ;
        for(int i = 0; i< elem.second.size(); i++)
        {
            std::cout << elem.second[i] <<" ";
        }
        std::cout << std::endl;
    }





///On parcourt compo
    for(auto elem: compo)
    {
        VertexInterface* interface;


        interface = add_interfaced_vertex(elem.first, 50, 50+elem.first, 500+elem.first, 2, "sommet_basique.png");
        Vertex v( 50, interface);
        ///On parcourt les sommets de la composante fortement connexe
        for(int i= 0; i< compo[elem.first].size(); i++)
        {


            ///On parcourt les m_in de chaque sommet
            for(int j= 0; j< m_vertices[compo[elem.first][i]].m_in.size(); j++)
            {
                ///Et on les ajoute dans in (tableau intermédiaire)

                v.m_in.push_back(m_vertices[compo[elem.first][i]].m_in[j]);

            }
            ///On parcourt les m_out de chaque sommet
            for(int j= 0; j< m_vertices[compo[elem.first][i]].m_out.size(); j++)
            {
                ///Et on les ajoute dans out (tableau intermédiaire)
                v.m_out.push_back(m_vertices[compo[elem.first][i]].m_out[j]);


            }



        }
        m_vertices_fortement_connexe.insert(std::pair<int, Vertex>(elem.first, v));

    }



    int cpt = 0;
    for(auto elem : m_vertices_fortement_connexe)
    {

        for(auto elem2 : m_vertices_fortement_connexe)
        {
            for(int i= 0; i< elem.second.m_in.size(); i++)
            {
                for(int k= 0; k< elem2.second.m_out.size(); k++)
                {
                    if(elem.second.m_in[i] == elem2.second.m_out[k] )
                    {
                        ///on cree une arete
                        Edge e;
                        ///on remplie ses m_to et m_from
                        e.m_from = elem2.first;
                        e.m_to = elem.first;
                        ///on l'affiche
                        add_interfaced_edge(cpt, e.m_from, e.m_to,2,  0);
                        ///on l'ajoute à la map d'arete

                        m_edges_fortement_connexe.insert ( std::pair<int,Edge>(cpt,e) );
                        cpt++;

                    }
                }
            }
            for(int i= 0; i< elem.second.m_out.size(); i++)
            {
                for(int k= 0; k< elem2.second.m_in.size(); k++)
                {
                    if(elem.second.m_out[i] == elem2.second.m_in[k] )
                    {
                        ///on cree une arete
                        Edge e;
                        ///on remplie ses m_to et m_from
                        e.m_from = elem.first;
                        e.m_to = elem2.first;
                        ///on l'affiche
                        add_interfaced_edge(cpt, e.m_from, e.m_to,2,  0);
                        ///on l'ajoute à la map d'arete

                        m_edges_fortement_connexe.insert ( std::pair<int,Edge>(cpt,e) );
                        cpt++;

                    }
                }
            }

        }

    }




}

int ** Graph::toutes_compo_forte_connexe()
{
    ///Variables locales
    int** tabc;///Tableau dynamique des composantes fortements connexes à retourner
    int* marques; ///Tableau de marquage
    int x, y; ///Numero des sommets intermédiares comme indice des tableaux intermédiairs
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



    ///Initialisation des valeurs à 0
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
    ///Pour tous les sommets x non marqués
    ///Recherche compo fortement connexe de x
    ///Marquer chaque sommet x et marquer les sommets y connectés a x non marqués
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
    int *c; ///Compo fortement connexe = intersection entre c1 et c2 ( à retourner)
    int * marques; ///tableau de marquages
    int x, y; ///numéros de sommets intermédiaires des composantes connexes
    int ajoute = 1; /// bool indiquant si une nouvelle composante connexe esr ajoutée


    ///Allocation tableau c1 et c2 et c et marque de taille ordre

    marques = new int[m_ordre];
    c1 = new int[m_ordre];
    c2= new int[m_ordre];
    c = new int[m_ordre];



    ///Initialisation valeurs tab à 0;
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

    ///Recherche des compo connexes partant de s à ajouter dans c1
    while(ajoute)
    {
        ajoute = 0; ///a chaque tour, recherche d'une nouvelle compo connexe a ajouter

        ///Pour tous les sommets x non marqués et connectés en partant de s
        ///Marquer chaque sommet x et connecter les sommets non marqués y adjacents à x
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
    ///On recherche les composantes connexes arrivant à s à ajouter dans c2
    while(ajoute)
    {

        ajoute = 0; ///a chaque tour, recherche d'une nouvelle compo connexe a ajouter

        ///Pour tous les sommets x non marqués et connectés en partant de s
        ///Marquer chaque sommet x et connecter les sommets non marqués y adjacents à x
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

     int save =0;

    ///bouton pause
   while (!key[KEY_SPACE])

    {


        ///a chaque tour de boucle la population croit ou decroit
        for(auto elem : m_vertices)
        {

            std::cout << "sommet suivant" <<elem.first<< std::endl;
            ///evolution en fonction du temps
            dynamique_pop2(elem.first);


       }
       update();
       save++;
       if(save ==3)
       {
           save_para();
       }
         grman::mettre_a_jour();



}

}

///methode gestion dynamique population 2

void Graph::dynamique_pop2(int sommet)
{




    int n; ///valeur (choisie par l'utilisateur) attribuée  au sommet


        ///calcul k
        for(int i =0; i<m_vertices[sommet].m_in.size(); i++)
        {


            k = k + m_edges[m_vertices[sommet].m_in[i]].m_weight * m_vertices[m_edges[m_vertices[sommet].m_in[i]].m_from].m_value;

        }

    if(k==0)
        k=1;

        std::cout<<" k = "<<k<<std::endl;

        ///calcul des éléments à soustraire
        for(int i =0; i<m_vertices[sommet].m_out.size(); i++)
        {
            c = c + m_edges[m_vertices[sommet].m_out[i]].m_weight * m_vertices[m_edges[m_vertices[sommet].m_out[i]].m_to].m_value;

        }

    std::cout <<" c = " << c << std::endl;
        ///on parcourt les aretes entrantes =  vecteur : m_vertices[sommet].m_in
        for(int i =0; i<m_vertices[sommet].m_out.size(); i++)
        {
            ///sommet voisin = vecteur :m_edges[m_vertices[sommet].m_in[i]].m_from



            ///valeur du sommet voisin = m_vertices[m_edges[m_vertices[sommet].m_in[i]].m_from].m_value
            n=m_vertices[m_edges[m_vertices[sommet].m_out[i]].m_to].m_value;


              std::cout << " r*n*(1 - n/k) = " << r*n*(1 - n/k) << std::endl;
            n = n +r*n*(1 - n/k) - c;

            ///suppression des cas de populations négatives (impossible physiquement) = on minore à zéro
            if (n <0)
            {
                n =0;
            }


            std::cout<<" n in= "<<n<<std::endl;


            ///On recupere la valeur de n dans l'attribut m_value du sommet pointé
            m_vertices[m_edges[m_vertices[sommet].m_out[i]].m_to].m_value = n;

            ///on maj la valeur du slider de notre sommet voisin
             m_vertices[m_edges[m_vertices[sommet].m_out[i]].m_to].m_interface->m_slider_value.set_value(n);



        }


        ///on parcourt les aretes entrantes =  vecteur : m_vertices[sommet].m_in
        for(int i =0; i<m_vertices[sommet].m_in.size(); i++)
        {



            ///valeur du sommet voisin = m_vertices[m_edges[m_vertices[sommet].m_in[i]].m_from].m_value
            n=m_vertices[m_edges[m_vertices[sommet].m_in[i]].m_from].m_value;


            n = n +r*n*(1 - n/k) - c;

            ///suppression des cas de populations négatives (impossible physiquement) = on minore à zéro
            if (n <0)
            {
                n =0;
            }





            ///On recupere la valeur de n dans l'attribut m_value du sommet pointé
            m_vertices[m_edges[m_vertices[sommet].m_in[i]].m_from].m_value = n;

            ///on maj la valeur du slider de notre sommet voisin
            m_vertices[m_edges[m_vertices[sommet].m_in[i]].m_from].m_interface->m_slider_value.set_value(n);


        }
    }

    ///methode sauvegarde parametre
void Graph::save_para()
{
  ///donnees a sauvegarder :m_ordre, index/sommet + coord + N, map aretes
    std::string m_save;

    std::cout<<"entrer le nom du fichier dans lequel vous souhaitez sauvegarder les parametres du graphe svp "<<std::endl;
    std::cin>>m_save;
    std::fstream fichier(m_save, std::ios::out|std::ios::app);

    if(fichier)
    {
        fichier.clear();
        fichier << r<<'\n';
        fichier << k<<'\n';
        fichier << c<<'\n';
        for(auto elem : m_vertices)
        {
            fichier<< elem.first<<' '<<m_vertices[elem.first ].m_value<<'\n';

        }

        fichier.close();
        std::cout<<" sauvegarde parametres effectuee"<<std::endl;

    }
    else
        std::cerr <<" impossible d'ouvrir ton fichier nigga "<<std::endl;
}


///methode gestion population
void Graph :: gestion()
{
    while(!key[KEY_SPACE])
    {
        ///pour chaque sommet
        for(auto elem : m_vertices)
        {
            ///je regarde mes arete entrantes
            for(int i=0; i<m_vertices[elem.first].m_in.size(); i++)
            {
                ///s'il n'a pas assez a manger
                if(m_edges[m_vertices[elem.first].m_in[i]].m_weight >m_vertices[m_edges[m_vertices[elem.first].m_in[i]].m_from].m_value)
                {
                    ///la populatio diminue
                    if( m_vertices[elem.first].m_value-5 >0)
                    {
                        m_vertices[elem.first].m_value = m_vertices[elem.first].m_value -5;
                        m_vertices[elem.first].m_interface->m_slider_value.set_value(m_vertices[elem.first].m_value);
                    }
                    else
                        m_vertices[elem.first].m_value = 0;

                }

                ///s'il a asez a manger
                if(m_edges[m_vertices[elem.first].m_in[i]].m_weight < m_vertices[m_edges[m_vertices[elem.first].m_in[i]].m_from].m_value)
                {
                    ///la population augmente
                    m_vertices[elem.first].m_value = m_vertices[elem.first].m_value + 5;
                    m_vertices[elem.first].m_interface->m_slider_value.set_value(m_vertices[elem.first].m_value);
                }
            }


        }
        update();
        grman::mettre_a_jour();
        rest(250);
    }
}


///Méthodes pour k-connexité
void Graph::changement_nb_desactive()
{
    ///On initialise les attributs
    for(auto elem : m_vertices)
    {
        m_vertices[elem.first].m_actif = true;
        m_vertices[elem.first].m_coul = ' ';
        m_vertices[elem.first].m_interface->m_top_box.set_bg_color(BLANC);

    }

    for(int i=0; i<m_sommets_retires.size(); i++)
    {
        m_sommets_retires.pop_back();
    }

    ///Nbre de composante connexe du graphe
    int connexe = 0;
    ///Tableau contenant les indices des sommets à supprimer pour déconnecter le graphe
    std::vector<int> deco;
    ///Nombre minimum de k_uplets sommets à enlever
    m_k_min = 0;


    ///Tant que k_m_in n'est pas trouvé
    while(m_k_min == 0)
    {

        ///On incrémente le nombre de k_uplets à regarder
        for( int k=1; k<m_ordre+1; k++)
        {
            ///on regarde la suppression d'un seul sommet à la fois
            ///Si le nombre k de k_uplet n'est pas trouvé
            if(k == 1 && m_k_min == 0)
            {
                std::cout << "k : " << k << std::endl;

                ///On parcourt les sommets
                for(auto elem: m_vertices)
                {


                    ///on désactive un sommet
                    m_vertices[elem.first].m_actif = false;
                    ///On calcul le nombre de composante connexe du graphe lorsque ce sommet est "supprimé"
                    connexe = colorier_compo_connexe();
                    ///On réactive le sommet
                    m_vertices[elem.first].m_actif = true;

                    ///Si le graphe a plus d'une composante connexe, donc s'il est déconnecté
                    if(connexe > 1)
                    {
                        ///On a trouvé le nombre de k_uplet minimum
                        m_k_min = k;
                        ///On insère le sommet à supprimer dans deco
                        deco.push_back(elem.first);

                    }
                }

                ///Affichage de k_min et des sommets à retirer sur l'interface et en console
                std::cout << "k_min :" << m_k_min << std::endl;
                for(int i = 0; i< deco.size(); i++)
                {
                    std::cout << "sommet a retirer " << deco[i] << std::endl;

                    m_vertices[deco[i]].m_interface->m_top_box.set_bg_color(ROUGE);

                }
                ///On remet à zéro deco
                for(int i = 0; i< deco.size(); i++)
                {
                    deco.pop_back();
                }
                ///Si le nombre de k-uplet pas trouvé quand on supprime les sommets un par un
                if(m_k_min == 0)
                {
                    ///On passe au k suivant
                    k=2;
                }
            }



            ///Si le nombre de k_uplet minimum n'est pas 1
            if(k != 1  && m_k_min == 0 )
            {
                std::cout << "k : " << k << std::endl;

                ///On remet à zéro les tableaux
                for (int i = 0; i < m_combinaison_partielle.size(); ++i)
                {
                    m_combinaison_partielle.pop_back();

                }
                for (int i = 0; i < m_combinaison_totale.size(); ++i)
                {
                    m_combinaison_totale.pop_back();

                }

                for (int i = 0; i < m_ordre; ++i)
                {
                    m_combinaison_partielle.push_back(i+1);

                }
                ///Si k_min toujours pas trouvé
                if(m_k_min == 0)
                {
                    ///on cherche toutes les combinaisons k parmis n
                    boucle_combinaison( 0, k, k);

                }



                ///On remet à zéro les tableaux
                for (int i = 0; i < m_combinaison_partielle.size(); ++i)
                {
                    m_combinaison_partielle.pop_back();

                }
                for (int i = 0; i < m_combinaison_totale.size(); ++i)
                {
                    m_combinaison_totale.pop_back();

                }

             int coul = COULEURALEATOIRESOMBRE;
    std::cout << "k_min :" << m_k_min << std::endl;
    for(int i = 0; i< m_sommets_retires.size(); i++)
    {
        std::cout << "sommet a retirer " << m_sommets_retires[i] << std::endl;

        m_vertices[m_sommets_retires[i]].m_interface->m_top_box.set_bg_color(coul);

    }


            }


        }



    }


    for(int i = 0; i< m_sommets_retires.size(); i++)
    {


       m_sommets_retires.pop_back();

    }

    std::cout << "fin calcul k_connexite" << std::endl;
    return;
}

void Graph::boucle_combinaison(int offset, int numero_de_k_uplets, int k)
{
    ///Si on a récupéré toutes les combinaisons de k parmis n
    if(numero_de_k_uplets == 0 )
    {
        ///On regarde numero de composantes connexes pour chacune
        k_connexite(k);
        return;

    }
    else
    {
        ///On crée des combinaisons
        for(int i=0; i< m_combinaison_partielle.size() - (numero_de_k_uplets); i++)
        {

            m_combinaison_totale.push_back(m_combinaison_partielle[i]);
            boucle_combinaison(i+1, numero_de_k_uplets -1, k);
            m_combinaison_totale.pop_back();

        }
    }



}


///Méthode  k-connexité
void Graph::k_connexite(int k)
{

    ///Nombre de composante connexe du graphe
    int connexe = 0;


    ///Affichage des combinaisons
    static int count = 0;
    std::cout << "combination no " << (++count) << ": [ ";
    for (int i = 0; i < m_combinaison_totale.size(); ++i)
    {
        std::cout << m_combinaison_totale[i] << " ";
    }
    std::cout << "] " << std::endl;



    ///Pour chaque combinaison
    for(int i=0; i< m_combinaison_totale.size(); i++)
    {
        ///on désactive les sommets
        m_vertices[m_combinaison_totale[i]].m_actif = false;

    }



    ///On cherche le nombre de composante connxe du graphe sans les sommets retirés
    connexe = colorier_compo_connexe();


    ///Pour chaque combinaison
    for(int i=0; i< m_combinaison_totale.size(); i++)
    {
        ///On réactive les sommets
        m_vertices[m_combinaison_totale[i]].m_actif = true;


    }
    for(int i=0; i<m_combinaison_totale.size(); i++)
    {
        ///si le nombre de composante connexe est supérieur à 1 donc que le graphe est déconnecté
        if(connexe > 1)
        {
            ///Le nombre de k-uplet minimum à enelever est k
            m_k_min = k;
            ///on ajoute les sommets de la combinaison dans deco
            m_sommets_retires.push_back( m_combinaison_totale[i]);



        }
    }






    return;
}




/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    m_interface->m_connexite.update();
    m_interface->m_tool_box.update();
    m_interface->m_sauvegarde.update();
    m_interface->m_animation1.update();
    m_interface->m_animation2.update();
    m_interface->m_ajout_a.update();
    m_interface->m_ajout_s.update();
    m_interface->m_fort_connexite.update();
    m_interface->m_k_connexite.update();
    m_interface->m_menu.update();

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_menu.update();
    m_interface->m_top_box.update();
    m_interface->m_tool_box.update();
    m_interface->m_sauvegarde.update();
    m_interface->m_connexite.update();
    m_interface->m_animation1.update();
    m_interface->m_animation2.update();
    m_interface->m_fort_connexite.update();
    m_interface->m_ajout_a.update();
    m_interface->m_ajout_s.update();
    m_interface->m_legende_sauvegarde.update();
    m_interface->m_k_connexite.update();


    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

    m_interface->m_tool_box.update();
    m_interface->m_sauvegarde.update();
    m_interface->m_animation1.update();
    m_interface->m_animation2.update();
    m_interface->m_connexite.update();
    m_interface->m_ajout_s.update();
    m_interface->m_ajout_a.update();
    m_interface->m_fort_connexite.update();
    m_interface->m_k_connexite.update();
    m_interface->m_menu.update();
}

/// Aide à l'ajout de sommets interfacés
VertexInterface * Graph::add_interfaced_vertex(int idx, double value, int x, int y, int graphe, std::string pic_name, int pic_idx )
{

    if(graphe == 1)
    {


        if ( m_vertices.find(idx)!=m_vertices.end() )
        {
            std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
            throw "Error adding vertex";
        }
        // Création d'une interface de sommet
        VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
        // Ajout de la top box de l'interface de sommet
        m_interface->m_main_box.add_child(vi->m_top_box);


        // On peut ajouter directement des vertices dans la map avec la notation crochet :
        m_vertices[idx] = Vertex(value, vi);
    }
    else
    {
        if ( m_vertices_fortement_connexe.find(idx)!=m_vertices_fortement_connexe.end() )
        {
            std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
            throw "Error adding vertex";
        }
        // Création d'une interface de sommet
        VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
        // Ajout de la top box de l'interface de sommet
        m_interface->m_main_box.add_child(vi->m_top_box);
        return vi;
    }
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2,int graphe, double weight)
{
    if(graphe == 1)
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
    else
    {
        if ( m_edges_fortement_connexe.find(idx)!= m_edges_fortement_connexe.end() )
        {
            std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
            throw "Error adding edge";
        }

        if ( m_vertices_fortement_connexe.find(id_vert1)==m_vertices_fortement_connexe.end() || m_vertices_fortement_connexe.find(id_vert2)==m_vertices_fortement_connexe.end() )
        {
            std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
            throw "Error adding edge";
        }

        EdgeInterface *ei = new EdgeInterface(m_vertices_fortement_connexe[id_vert1], m_vertices_fortement_connexe[id_vert2]);
        m_interface->m_main_box.add_child(ei->m_top_edge);
        m_edges_fortement_connexe[idx] = Edge(weight, ei, id_vert1, id_vert2);
    }
}


///getter m_monde
int Graph::GetMonde()
{
    return m_monde;
}

void Graph::SetMonde(int monde)
{
    m_monde =monde;
}

