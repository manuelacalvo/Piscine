#include "graph.h"
#include <fstream>

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0); // Valeurs arbitraires, � adapter...
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
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0 , 100.0); // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
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
                 sprintf(chaine,"banquise/%d.jpg",i);
                add_interfaced_vertex(i, 30.0*i, 200/i, 100/i, chaine);
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
                add_interfaced_vertex(i, 30.0*i, 200/i, 100/i, chaine);
               /* if(!decor[i])
                {
                    allegro_message("pas pu trouver");
                }*/

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
           int s1,s2;
           int poids;
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


        //fermeture du fichier
        fichier.close();



        //Si erreur d'ouverture
    }
    else
        std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;

}

/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
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
    m_edges[idx] = Edge(weight, ei);
}

