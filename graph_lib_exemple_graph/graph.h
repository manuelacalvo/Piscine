#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

/**************************************************************
    Ici sont propos�es 3 classes fondamentales
            Vertex (=Sommet)
            Edge (=Ar�te ou Arc)
            Graph (=Graphe)

    Les ar�tes et les sommets et le graphe qu'ils constituent
    "travaillent" �troitement ensemble : pour cette raison les
    Vertex et Edge se d�clarent amis (friend) de Graph pour que
    ce dernier puisse librement acc�der aux membres (y compris
    protected ou private) de Vertex et Edge.

    Ces Classes peuvent �tres compl�t�es. Il est �galement possible
    de les d�river mais il est malheureusement assez difficile
    de d�river le "triplet" des 3 classes en maintenant des relations
    coh�rentes ( rechercher "c++ class covariance" et "c++ parallel inheritance"
    pour commencer .. ). Il est donc sans doute pr�f�rable, si possible,
    de "customiser" ces classes de base directement, sans h�ritage.

    Le mod�le propos� permet de repr�senter un graphe orient� �ventuellement
    pond�r�, les arcs portent une ou des informations suppl�mentaire(s).
    Les relations/navigations Arcs -> Sommets et Sommets -> Arcs se font
    dans les 2 sens et utilisent des INDICES et NON PAS DES ADRESSES (pointeurs)
    de telle sorte que la topologie du graphe puisse �tre assez facilement
    lue et �crite en fichier, et b�n�ficie d'une bonne lisibilit� en cas de bugs...

    Chaque arc poss�de 2 attributs principaux (en plus de son �ventuelle pond�ration)
        -> m_from (indice du sommet de d�part de l'arc )
        -> m_to (indice du sommet d'arriv�e de l'arc )

    Chaque sommet poss�de 2 liste d'arcs (en plus de ses attributs "internes", marquages...)
        -> m_in (liste des indices des arcs arrivant au sommet : acc�s aux pr�d�cesseurs)
        -> m_out (liste des indices des arcs partant du sommet : acc�s aux successeurs)

    Cependant le probl�me des indices (par rapport aux pointeurs) et qu'en cas
    de destruction d'une entit� (un arc et/ou un sommet sont enlev�s du graphe) alors :

    - Soit il faut reprendre toute la num�rotation pour "boucher le trou"
      (par exemple on a supprim� le sommet n�4, le sommet n�5 devient le 4, 6 devient 5 etc...)
      ce qui pose des probl�mes de stabilit� et de coh�rence, et une difficult� � r�-introduire
      le(s) m�me(s) �l�ment supprim� (au m�me indice)

    - Soit on admet que la num�rotation des sommets et arcs n'est pas contigue, c�d qu'il
      peut y avoir des "trous" : sommets 0 1 5 7 8, pas de sommets 2 ni 3 ni 4 ni 6. La num�rotation
      est stable mais on ne peut plus utiliser un simple vecteur pour ranger la liste de tous
      les arcs et tous les sommets aux indices correspondants, on peut utiliser une map
      qui associe un objet arc ou sommet � des indices arbitraires (pas forc�ment contigus)

    C'est cette 2�me approche qui est propos�e ici : dans la classe graphe vous trouverez
        -> map<int, Edge>   m_edges
        -> map<int, Vertex> m_vertices    (le pluriel de vertex est vertices)

    Il faudra �tre attentif au fait que par rapport � un simple vecteur, le parcours des �l�ments
    ne pourra PAS se faire avec un simple for (int i=0; i<m_edges.size(); ++i) ...m_edges[i]...
    et que les parcours � it�rateur ne donneront pas directement des Edge ou des Vertex
    mais des pairs, l'objet d'int�r�t se trouvant dans "second" ("first" contenant l'indice)
                for (auto &it = m_edges.begin(); it!=m_edges.end(); ++it) ...it->second...
    ou bien     for (auto &e : m_edges) ...e.second...

    Il n'est pas obligatoire d'utiliser ces classes pour le projet, vous pouvez faire les votres

    Au niveau de l'interface, on dissocie une classe interface associ�e � chaque classe fondamentale
    de telle sorte qu'il soit possible de travailler avec des graphes non repr�sent�s � l'�cran
    Imaginons par exemple qu'on doive g�n�rer 1000 permutations de graphes pour tester des
    combinaisons, on ne souhaite pas repr�senter graphiquement ces 1000 graphes, et les
    interfaces p�sent lourd en ressource, avec cette organisation on est libre de r�server ou
    pas une interface de pr�sentation associ�e aux datas (d�couplage donn�es/interface)

***********************************************************************************************/

#include <vector>
#include <map>
#include <string>
#include <memory>

#include "grman/grman.h"

/***************************************************
                    VERTEX
****************************************************/

///graphisme
class VertexInterface
{
    // Les (methodes des) classes amies pourront acc�der
    // directement aux attributs (y compris priv�s)
    friend class Vertex;
    friend class EdgeInterface;
    friend class Graph;

    private :

        /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de d�clarer
        /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
        /// le param�trer ( voir l'impl�mentation du constructeur dans le .cpp )

        // La boite qui contient toute l'interface d'un sommet
        grman::WidgetBox m_top_box;

        // Un slider de visualisation/modification de la valeur du sommet
        grman::WidgetVSlider m_slider_value;

        // Un label de visualisation de la valeur du sommet
        grman::WidgetText m_label_value;

        // Une image de "remplissage"
        grman::WidgetImage m_img;

        // Un label indiquant l'index du sommet
        grman::WidgetText m_label_idx;

        // Une boite pour le label pr�c�dent
        grman::WidgetText m_box_label_idx;

        ///CLICK ME , etc
        grman::WidgetText m_legende;
        grman::WidgetCheckBox m_marche;



    public :

        // Le constructeur met en place les �l�ments de l'interface
        // voir l'impl�mentation dans le .cpp
        VertexInterface(int idx, int x, int y, std::string pic_name="", int pic_idx=0);
};


class Vertex
{
    // Les (methodes des) classes amies pourront acc�der
    // directement aux attributs (y compris priv�s)
    friend class Graph;
    friend class VertexInterface;
    friend class Edge;
    friend class EdgeInterface;

    private :
        /// liste des indices des arcs arrivant au sommet : acc�s aux pr�d�cesseurs
        std::vector<int> m_in;

        /// liste des indices des arcs partant du sommet : acc�s aux successeurs
        std::vector<int> m_out;

        /// un exemple de donn�e associ�e � l'arc, on peut en ajouter d'autres...
        double m_value;
        bool m_actif;

        /// le POINTEUR sur l'interface associ�e, nullptr -> pas d'interface
        std::shared_ptr<VertexInterface> m_interface = nullptr;

        // Docu shared_ptr : https://msdn.microsoft.com/fr-fr/library/hh279669.aspx
        // La ligne pr�c�dente est en gros �quivalent � la ligne suivante :
        // VertexInterface * m_interface = nullptr;

        ///couleur pour la connexite
        char m_coul;

        ///coordon�es sommet
        int x;
        int y;

    public:

        /// Les constructeurs sont � compl�ter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Vertex (double value=0, VertexInterface *interface=nullptr) :
            m_value(value), m_interface(interface), m_coul(' '),x(0),y(0), m_actif(true)  {  }

        Vertex (double value, VertexInterface *interface, std::vector<int> _in, std::vector<int> _out) :
            m_value(value), m_interface(interface), m_in(_in), m_out(_out)  {  }
        /// Vertex �tant g�r� par Graph ce sera la m�thode update de graph qui appellera
        /// le pre_update et post_update de Vertex (pas directement la boucle de jeu)
        /// Voir l'impl�mentation Graph::update dans le .cpp
        void pre_update();
        void post_update();
};



/***************************************************
                    EDGE
****************************************************/

class EdgeInterface
{
    // Les (methodes des) classes amies pourront acc�der
    // directement aux attributs (y compris priv�s)
    friend class Edge;
    friend class Graph;

    private :

        /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de d�clarer
        /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
        /// le param�trer ( voir l'impl�mentation du constructeur dans le .cpp )

        // Le WidgetEdge qui "contient" toute l'interface d'un arc
        grman::WidgetEdge m_top_edge;

        // Une boite pour englober les widgets de r�glage associ�s
        grman::WidgetBox m_box_edge;

        // Un slider de visualisation/modification du poids valeur de l'arc
        grman::WidgetVSlider m_slider_weight;

        // Un label de visualisation du poids de l'arc
        grman::WidgetText m_label_weight;

        grman::WidgetCheckBox m_marche;

    public :

        // Le constructeur met en place les �l�ments de l'interface
        // voir l'impl�mentation dans le .cpp
        EdgeInterface(Vertex& from, Vertex& to);
};


class Edge
{
    // Les (methodes des) classes amies pourront acc�der
    // directement aux attributs (y compris priv�s)
    friend class Graph;
    friend class EdgeInterface;

    private :
        /// indice du sommet de d�part de l'arc
        int m_from;

        /// indice du sommet d'arriv�e de l'arc
        int m_to;

        /// un exemple de donn�e associ�e � l'arc, on peut en ajouter d'autres...
        double m_weight;


        /// le POINTEUR sur l'interface associ�e, nullptr -> pas d'interface
        std::shared_ptr<EdgeInterface> m_interface = nullptr;


    public:

        /// Les constructeurs sont � compl�ter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Edge (double weight=0, int _from = 0, int _to = 0) :
            m_weight(weight), m_interface(nullptr) , m_from (_from), m_to (_to) {  }

        Edge (double weight, EdgeInterface *interface,int _from, int _to) :
            m_weight(weight), m_interface(interface) , m_from (_from), m_to (_to)  {  }
        /// Edge �tant g�r� par Graph ce sera la m�thode update de graph qui appellera
        /// le pre_update et post_update de Edge (pas directement la boucle de jeu)
        /// Voir l'impl�mentation Graph::update dans le .cpp
        void pre_update();
        void post_update();
};




/***************************************************
                    GRAPH
****************************************************/

class GraphInterface
{
    friend class Graph;

    private :

        /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de d�clarer
        /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
        /// le param�trer ( voir l'impl�mentation du constructeur dans le .cpp )

        /// La boite qui contient toute l'interface d'un graphe
        grman::WidgetBox m_top_box;

        /// Dans cette boite seront ajout�s les (interfaces des) sommets et des arcs...
        grman::WidgetBox m_main_box;

        /// Dans cette boite seront ajout�s des boutons de contr�le etc...
        grman::WidgetBox m_tool_box;

          ///case permettant la sauvegarde

        grman::WidgetEdge m_lien3;
         grman::WidgetText m_legende_sauvegarde;
        grman::WidgetText m_legende_connexite;
        grman::WidgetText m_legende_animation;
        grman::WidgetText m_legende_fort_connexite;
        grman::WidgetText m_legende_k_connexite;
        grman::WidgetText m_legende_menu;

        // A compl�ter �ventuellement par des widgets de d�coration ou
        // d'�dition (boutons ajouter/enlever ...)

    public :

    grman::WidgetButton m_menu;
    grman::WidgetButton m_k_connexite;
    grman::WidgetButton m_sauvegarde;
    grman::WidgetButton m_connexite;
    grman::WidgetButton m_animation;
    grman::WidgetButton m_ajout_s;
    grman::WidgetButton m_ajout_a;
    grman::WidgetButton m_fort_connexite;;
    grman::WidgetText m_ajout_s_text;
     grman::WidgetText m_ajout_a_text;
    grman::WidgetText m_connexite_text;


        // Le constructeur met en place les �l�ments de l'interface
        // voir l'impl�mentation dans le .cpp
        GraphInterface(int x, int y, int w, int h);

};


class Graph
{
    private :

        /// La "liste" des ar�tes
        std::map<int, Edge> m_edges;

        /// La liste des sommets
        std::map<int, Vertex> m_vertices;

        /// ordre
        int m_ordre;
        int m_arete;
        int** m_adjacence;
        std::vector<int> m_combinaison_totale;
        std::vector<int> m_combinaison_partielle;
        bool m_trouve;



        /// le POINTEUR sur l'interface associ�e, nullptr -> pas d'interface
        std::shared_ptr<GraphInterface> m_interface = nullptr;
        int m_monde;




    public:

        int GetMonde();
        void SetMonde(int monde);

        /// Les constructeurs sont � compl�ter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Graph (GraphInterface *interface=nullptr)
        : m_interface(interface), m_ordre(0), m_arete(0), m_trouve(false), m_monde(0)
        {


        }

        ~Graph()
        {
             //Destruction matrice d'adjacence
            for(int i=0; i<m_ordre; i++ )
            {
        delete[] m_adjacence[i];
            }
        delete[] m_adjacence;
        }

        void add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name="", int pic_idx=0 );
        void add_interfaced_edge(int idx, int vert1, int vert2, double weight=0);

        std::shared_ptr<GraphInterface> get_interface();

        ///******NOS METHODES******///
        ///edition des reseaux par l'utilisateur
        int editer_reseaux(int choix2 );

        ///methode pour recuperer les aretes des graphes
        void recup_arete(std::string _nom);
        void remplissage_min_mout();

        ///methode pour sauvegarder les graphes
        void sauvegarde();


        ///methode pour recuperer les graphes sauvegard�s
        void recup_sommet(std::string _nom);

        ///methode suppression arete
        void supp_arete_test();
        void supp_arete(int eidx);

        ///methode suppression sommet
        void supp_sommet_selec();
        void supp_sommet(int vdx);

        ///methode ajout sommet
        void ajout_sommet();
        void choix_photo(int s_ajout);

        ///methode ajout arete
        void ajout_arete(int s_ajout, int s_relie);
        void ajout_arete_selec();

        ///methode dynamique pop
        void dynamique_pop1();
        void dynamique_pop2(int sommet);

        ///methode �tude connexit�
        void colorier_som(Vertex v, char coul);
        int colorier_compo_connexe();
        int* Une_Compo_Forte_Connexe( int s, int chiffre);
        void remplir_matrice_adjacence();
        int ** toutes_compo_forte_connexe();
        void fortement_connexe();
        void k_connexite(int k);
        void changement_nb_desactive();
        void boucle_combinaison(int offset, int numero_de_k_uplets, int k);
        int facto(int n);
        int combi(int k);


        /// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
        void update();
};


#endif // GRAPH_H_INCLUDED
