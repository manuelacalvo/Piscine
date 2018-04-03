#include "graph.h"
#include <fstream>

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
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


/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
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
    m_box_edge.set_bg_color(BLANCBLEU);

    /// Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

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
    /* m_top_box.add_child(m_tool_box);
      m_tool_box.set_dim(40,20);
     m_tool_box.set_gravity_xy(grman::GravityX::Left,grman::GravityY::Down);
     m_tool_box.set_bg_color(ORANGE);
*/
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);


}


void Graph::editer_reseaux(int choix)
{
 m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
 //m_interface->m_sauvegarde.add_chill(m_top_box);
    switch(choix)
    {

        ///banquise
    case 1 :
        {


            char chaine[256];
            BITMAP*decor[12];
            for(int i =1;i<12;i++)
            {
                 ///charger et afficher un sommet
                 Vertex sommet;
                 sprintf(chaine,"banquise/%d.jpg",i);

                add_interfaced_vertex(i, 30.0*i, 200/i, 100/i, chaine);
               // m_vertices[i]=
               /* if(!decor[i])
                {
                    allegro_message("pas pu trouver");
                }*/

            }



       recup_arete("aretes_banquise.txt");

            break;
        }

        ///savane
    case 2 :
        {


            char chaine[256];
            BITMAP*decor[15];
            for(int i =1;i<15;i++)
            {
                 ///charger et afficher un sommet
                 sprintf(chaine,"savane/%d.jpg",i);
                add_interfaced_vertex(i, 30.0*i, 200/i, 100/i, chaine);
               /* if(!decor[i])
                {
                    allegro_message("pas pu trouver");
                }*/

            }


       recup_arete("aretes_savane.txt");
           break;
        }

        ///foret
    case 3 :
        {
             char chaine[256];
            BITMAP*decor[15];
            for(int i =1;i<15;i++)
            {
                 ///charger et afficher un sommet
                 sprintf(chaine,"foret/%d.jpg",i);

            }


       recup_arete("aretes_foret.txt");
           break;
        }

    }
}

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
           int poids = 0;

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


        ///on parcourt tous les sommets
         for(auto elem2 : m_vertices)
         {
 Vertex v;
            ///on parcours les aretes
             for(auto elem : m_edges)
             {
                 ///si le 1er sommet de l'arete est le meme que notre sommet
                 if((elem.second).m_from == elem2.first)
                 {
                     ///on ajoute l'indice de cette arete dans le vec d'aretes sortantes
                     (v.m_out).push_back(elem.first);
                 }

                ///si le 2eme sommet de l'arete est le meme que notre sommet
                 if((elem.second).m_to == elem2.first)
                 {
                     ///on ajoute l'indice de cette arete dans le vec d'aretes entrantes
                     (v.m_in).push_back(elem.first);
                 }
                  m_vertices[elem2.first]= v;


             }




         }
        //fermeture du fichier
        fichier.close();



        //Si erreur d'ouverture
    }
    else
        std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;

}

void Graph::sauvegarde()
{

}

void Graph::colorier_som(Vertex v, char coul)
{
    std::cout << "dans colorier sommet " << std::endl;
    ///on attribue une couleur au sommet actuel
    v.m_coul = coul;
    std::cout<<"couleur "<<coul<<std::endl;


    ///On parcours les aretes sortantes du sommet v
    std::cout<<" v.m_out.size() "<<v.m_out.size()<<std::endl;
    if(v.m_out.size()!=0)
    {
       for(int i=0; i<v.m_out.size(); i++)
        {
            std::cout<<" dans la boucle out"<<std::endl;
                for(auto elem: m_vertices)
                {
                    if(m_vertices[elem.first].m_in.size()!=0)
                       { std::cout<<" m_vertices[elem.first].size() "<< m_vertices[elem.first].m_in.size() << std::endl;
                           for(int j =0; j<m_vertices[elem.first].m_in.size();j++)
                            {
                        std::cout<<" (elem.second).m_in[j]  "<<(elem.second).m_in[j] <<" v.m_out[i] "<<v.m_out[i]<<std::endl;
                        if((elem.second).m_in[j]  == v.m_out[i])
                        {
                            if(m_vertices[elem.first].m_coul == ' ' )
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
     std::cout<<" v.m_in.size() "<<v.m_in.size()<<std::endl;
       if(v.m_in.size()!=0)
       {
           ///on parcourt les indices d'aretes entrantes de v
           for(int i=0; i<v.m_in.size(); i++)
        {
            std::cout<<" dans la boucle in"<<std::endl;
            ///on parcourt les sommets du graphe
                for(auto elem: m_vertices)
                {
                    if(m_vertices[elem.first].m_out.size()!=0)
                    {
                                            ///on parcourt les aretes sortantes de v
                    for(int j =0; j<m_vertices[elem.first].m_out.size();j++)
                    {
                        ///si l'indice
                        if((elem.second).m_out[j]  == v.m_in[i])
                        {
                            std::cout<<" (elem.second).m_out[j]  "<<(elem.second).m_out[j] <<" v.m_in[i] "<<v.m_in[i]<<std::endl;
                            if(m_vertices[elem.first].m_coul == ' ' )
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
void Graph ::colorier_compo_connexe()
{
    ///variable de couleur
    char coul = 'A';

    ///compteur nb compo connexes
    int nb_compo = 0;

    ///on parcourt les sommets
    for(auto elem : m_vertices)
    {
        ///si le sommet n'est pas colorié
        if ((m_vertices[(elem.first)].m_coul) == ' ')
        {  std::cout << "Sommet " << elem.first << std::endl;

            ///on choisi une nouvelle couleur
            coul++;
            ///on incrémente le nb de compo connexe
                nb_compo++;

            ///on colorie le sommet avec la nouvelle couleur
            m_vertices[(elem.first)].m_coul = coul;

            ///on appelle la methode colorier sommet avec parametres le sommet et la new couleur
            colorier_som(elem.second, coul);
        }
    }
    std::cout << " nb compo : " << nb_compo << std::endl;
}




/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
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

/// Aide à l'ajout d'arcs interfacés
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

