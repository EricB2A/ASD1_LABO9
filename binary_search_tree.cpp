//
//  Binary Search Tree
//
//  Copyright (c) 2017 Olivier Cuisenaire. All rights reserved.
//

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <queue>
#include <cassert>
#include <stdexcept>

using namespace std;

template <typename T>
class BinarySearchTree
{
public:
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;

private:
    /**
     *  @brief Noeud de l'arbre.
     *
     * contient une cle et les liens vers les sous-arbres droit et gauche.
     */
    struct Node
    {
        const value_type key; // clé non modifiable
        Node *right;          // sous arbre avec des cles plus grandes
        Node *left;           // sous arbre avec des cles plus petites
        size_t nbElements;    // nombre de noeuds dans le sous arbre dont
        // ce noeud est la racine

        Node(const_reference key) // seul constructeur disponible. key est obligatoire
            : key(key), right(nullptr), left(nullptr), nbElements(1)
        {
            cout << "(C" << key << ") ";
        }
        ~Node() // destructeur
        {
            cout << "(D" << key << ") ";
        }
        Node() = delete;             // pas de construction par défaut
        Node(const Node &) = delete; // pas de construction par copie
        Node(Node &&) = delete;      // pas de construction par déplacement
    };

    /**
     *  @brief  Racine de l'arbre. nullptr si l'arbre est vide
     */
    Node *_root;

public:
    /**
     *  @brief Constructeur par défaut. Construit un arbre vide
     */
    BinarySearchTree() : _root(nullptr)
    {
    }

    /**
     *  @brief Constucteur de copie.
     *
     *  @param other le BinarySearchTree à copier
     *
     */
    BinarySearchTree(const BinarySearchTree &other)
    {
        if(other._root){
            try{
                _root = new Node(other._root->key);
                copyTree(other._root, _root);
            }
            catch(...){
                deleteSubTree(_root);
                _root = nullptr;
                throw;
            }
        }
    }

    /**
     *  @brief Opérateur d'affectation par copie.
     *
     *  @param other le BinarySearchTree à copier
     *
     */
    BinarySearchTree &operator=(const BinarySearchTree &other)
    {
        Node* root = nullptr;
        if(other._root){
            root = new Node(other._root->key);
            copyTree(other._root,root);
        }
        _root = root;
        return *this;
    }

    /**
     *  @brief Echange le contenu avec un autre BST
     *
     *  @param other le BST avec lequel on echange le contenu
     *
     */
    void swap(BinarySearchTree &other) noexcept
    {
        Node* temp = _root;
        _root = other._root;
        other._root = temp;
    }

    /**
     *  @brief constructeur de copie par déplacement
     *
     *  @param other le BST dont on vole le contenu
     *
     */
    BinarySearchTree(BinarySearchTree &&other) noexcept
    {
        _root = other._root;
        _root->nbElements = other._root->nbElements;
        other._root = nullptr;
    }

    /**
     *  @brief Opérateur d'affectation par déplacement.
     *
     *  @param other le BST dont on vole le contenu
     *
     */
    BinarySearchTree &operator=(BinarySearchTree &&other) noexcept
    {
        _root = other._root;
        other._root = nullptr;
        return *this;
    }

    //
    // @brief Destructeur
    //
    // Ne pas modifier mais écrire la fonction
    // récursive privée deleteSubTree(Node*)
    //
    ~BinarySearchTree()
    {
        deleteSubTree(_root);
    }

private:
    //
    //@brief copie un BTS dans un autre
    //@param src Racine du BTS a copier
    //@param dest Racine du BTS copié
    void copyTree(Node* src, Node* dest){
        if(src){
            dest->nbElements = src->nbElements;
            if(src->left){
                Node* leftNode = new Node(src->left->key);
                dest->left = leftNode;
                copyTree(src->left, dest->left);
            }
            if(src->right){
                Node* rightNode = new Node(src->right->key);
                dest->right = rightNode;
                copyTree(src->right, dest->right);
            }
        }
    }

    //
    // @brief Fonction détruisant (delete) un sous arbre
    //
    // @param r la racine du sous arbre à détruire.
    //          peut éventuellement valoir nullptr
    //
    static void deleteSubTree(Node *r) noexcept
    {
        if(r)
        {
            deleteSubTree(r->left);
            deleteSubTree(r->right);
            delete r;
        }
    }

public:
    //
    // @brief Insertion d'une cle dans l'arbre
    //
    // @param key la clé à insérer.
    //
    // Ne pas modifier mais écrire la fonction
    // récursive privée insert(Node*&,const_reference)
    //
    void insert(const_reference key)
    {
        insert(_root, key);
    }

private:
    //
    // @brief Insertion d'une cle dans un sous-arbre
    //
    // @param r la racine du sous-arbre dans lequel
    //          insérer la cle.
    // @param key la clé à insérer.
    //
    // @return vrai si la cle est inseree. faux si elle etait deja presente.
    //
    // Si la cle est deja presente, cette fonction ne fait rien.
    // x peut éventuellement valoir nullptr en entrée.
    // la fonction peut modifier x, reçu par référence, si nécessaire
    //
    static bool insert(Node *&r, const_reference key)
    {
        if(contains(r, key)){
            return false;
        }
        //Si l'arbre est vide
        else if (r == nullptr)
        {
            r = new Node(key);
            return true;
        }
        else if (key > r->key)
        {
            if (r->right == nullptr)
            {
                //insertion de la nouvelle feuille
                r->right = new Node(key);
                r->nbElements++;
                return true;
            }
            else{
                if(insert(r->right, key)){
                    r->nbElements++;
                }
                return true;
            }
        }
        else if (key < r->key)
        {
            if (r->left == nullptr)
            {
                //insertion de la nouvelle feuille
                r->left = new Node(key);
                r->nbElements++;
                return true;
            }
            else{
                if(insert(r->left, key)){
                    r->nbElements++;
                }
                return true;
            }
        }
    }

public:
    //
    // @brief Recherche d'une cle.
    //
    // @param key la cle a rechercher
    //
    // @return vrai si la cle trouvee, faux sinon.
    //
    // Ne pas modifier mais écrire la fonction
    // récursive privée contains(Node*,const_reference)
    //
    bool contains(const_reference key) const noexcept
    {
        return contains(_root, key);
    }

private:
    //
    // @brief Recherche d'une cle dans un sous-arbre
    //
    // @param key la cle a rechercher
    // @param r   la racine du sous-arbre
    //
    // @return vrai si la cle trouvee, faux sinon.
    //
    static bool contains(Node *r, const_reference key) noexcept
    {
        //Si la racine pointe sur null l'arbre est soit vide, soit on a pas trouvé la valeur
        if (r == nullptr)
            return false;
        else if (key == r->key)
            return true;
        //si la valeur est plus grande que la clé de la racine on va verifier dans le sous arbre droit
        else if (key > r->key)
            contains(r->right, key);
        //Sinon dans le sous arbre gauche
        else
            contains(r->left, key);
    }

    static Node *minNode(Node *r)
    {
        if (r == nullptr)
            throw std::logic_error("L'arbre est vide il n'y a donc pas de minimum");
        else
        {
            if (r->left == nullptr)
                return r;
            else
                minNode(r->left);
        }
    }

public:
    //
    // @brief Recherche de la cle minimale.
    //
    // @return une const reference a la cle minimale
    //
    // @exception std::logic_error si necessaire
    //
    // vous pouvez mettre en oeuvre de manière iterative ou recursive a choix
    //
    const_reference min() const
    {
        this->minNode(_root)->key;
    }

    //
    // @brief Supprime le plus petit element de l'arbre.
    //
    // @exception std::logic_error si necessaire
    //
    // vous pouvez mettre en oeuvre de manière iterative ou recursive a choix
    //
    void deleteMin()
    {
        Node *noeud = _root;
        if (noeud == nullptr)
        {
            throw std::logic_error("Arbre vide il n'est pas possible de delete le min");
        }
        if (noeud->left == nullptr)
        {
            _root = noeud->right;
            delete (noeud);
            return;
        }
        while (noeud->left->left != nullptr)
        {
            noeud->nbElements--;
            noeud = noeud->left;
        }
        noeud->nbElements--;
        Node *temp = noeud->left;
        noeud->left = noeud->left->right;
        noeud = nullptr;
       // _root->nbElements--;
        delete noeud;
        delete temp;
    };

    //
    // @brief Supprime l'element de cle key de l'arbre.
    //
    // @param key l'element a supprimer
    //
    // si l'element n'est pas present, la fonction ne modifie pas
    // l'arbre mais retourne false. Si l'element est present, elle
    // retourne vrai
    //
    // Ne pas modifier mais écrire la fonction
    // récursive privée deleteElement(Node*&,const_reference)
    //
    bool deleteElement(const_reference key) noexcept
    {
        return deleteElement(_root, key);
    }

private:
    static Node *&_minNode(Node *&r)
    {
        if(r == nullptr){
            throw std::logic_error("Arbre vide.");
        }
        if (r->left != nullptr)
        {
            r->nbElements--;
            return _minNode(r->left);
        }
        return r;
    }
    //
    // @brief Supprime l'element de cle key du sous arbre.
    //
    // @param r la racine du sous arbre
    // @param key l'element a supprimer
    //
    // si l'element n'est pas present, la fonction ne modifie pas
    // l'arbre mais retourne false. Si l'element est present, elle
    // retourne vrai
    //
    static bool deleteElement(Node *&r, const_reference key) noexcept
    {
        if (r == nullptr || !contains(r,key))
        {
            return false;
        }
        else if (key < r->key)
        {
            if(deleteElement(r->left, key))
                r->nbElements--;
            return true;
        }
        else if (key > r->key)
        {
            if(deleteElement(r->right, key))
                r->nbElements--;
            return true;
        }
        else if (r->key == key)
        {
            Node *tmp = r;
            if (r->left == nullptr)
            {
                std::swap(r, r->right);
                delete tmp;
                return true;
            }
            else if (r->right == nullptr)
            {
                std::swap(r, r->left);
                delete tmp;
                return true;
            }
            else
            {
                Node *&min = _minNode(r->right);
                size_t nbElems = r->nbElements -1;
                r = min;
                min = min->right;

                r->left = tmp->left;
                r->right = tmp->right;

                r->nbElements = nbElems;

                delete tmp;
                return true;
            }
        }
        return true;
    }

public:
    //
    // @brief taille de l'arbre
    //
    // @return le nombre d'elements de l'arbre
    //
    size_t size() const noexcept
    {

        return _root ? _root->nbElements : 0;
    }

    //
    // @brief cle en position n
    //
    // @return une reference a la cle en position n par ordre croissant des
    // elements
    //
    // @exception std::logic_error si nécessaire
    //
    // ajoutez le code de gestion des exceptions, puis mettez en oeuvre
    // la fonction recursive nth_element(Node*, n)
    //
    const_reference nth_element(size_t n) const
    {
        if(_root->nbElements < n){
            throw std::out_of_range("L'arbre ne contient pas autant d'elements");
        }
        return nth_element(_root, n);
    }

private:
    //
    // @brief cle en position n dans un sous arbre
    //
    // @param r la racine du sous arbre. ne peut pas etre nullptr
    // @param n la position n
    //
    // @return une reference a la cle en position n par ordre croissant des
    // elements
    //
    static const_reference nth_element(Node *r, size_t n) noexcept
    {
        assert(r != nullptr);
        size_t nbElementsGauche = 0;

        if (r->left){
            nbElementsGauche = r->left->nbElements;
        }

        //nième element dans le sous arbre gauche
        if (n > nbElementsGauche) {
            return nth_element(r->right, n - nbElementsGauche - 1);
        }
        //nième element dans le sous arbre droit
        else if (n < nbElementsGauche) {
            return nth_element(r->left, n);
        }
        //nième element est la racine
        else {
            return r->key;
        }
    }

public:
    //
    // @brief position d'une cle dans l'ordre croissant des elements de l'arbre
    //
    // @param key la cle dont on cherche le rang
    //
    // @return la position entre 0 et size()-1, size_t(-1) si la cle est absente
    //
    // Ne pas modifier mais écrire la fonction
    // récursive privée rank(Node*,const_reference)
    //
    size_t rank(const_reference key) const noexcept
    {
        return rank(_root, key);
    }

private:
    //
    // @brief position d'une cle dans l'ordre croissant des elements du sous-arbre
    //
    // @param key la cle dont on cherche le rang
    // @param r la racine du sous arbre
    //
    // @return la position entre 0 et size()-1, size_t(-1) si la cle est absente
    //
    static size_t rank(Node *r, const_reference key) noexcept
    {
        if (contains(r, key) && r!= nullptr) {
            size_t nbElementsGauche = 0;

            if (r->left) {
                nbElementsGauche += r->left->nbElements;
            }

            if (key > r->key)
                return rank(r->right, key) + nbElementsGauche + 1;
              else if (key < r->key)
                return rank(r->left, key);
              else
                return nbElementsGauche;
            return -1;
        }
        else{
            return -1;
        }
    }

public:
    //
    // @brief linearise l'arbre
    //
    // transforme l'arbre en une liste simplement chainee composee des memes
    // noeuds que precedemment, mais dont les pointeurs left sont tous egaux
    // a nullptr. Cette liste doit toujours respecter les conditions d'un
    // arbre binaire de recherche
    //
    // Ne pas modifier cette fonction qui sert essentiellement a tester la
    // fonction recursive linearize(Node*, Node*&, size_t&) utilisée par
    // la methode publique arborize
    //
    void linearize() noexcept
    {
        size_t cnt = 0;
        Node *list = nullptr;
        linearize(_root, list, cnt);
        _root = list;
    }

private:
    //
    // @brief linearise une sous arbre tree en une liste
    //
    // @param tree pointe vers la racine de l'arbre a lineariser
    // @param list reference a la tete de la liste a creer. sera modifiee
    //             par la fonction pour que list pointe vers le plus petit
    //             element de tree
    // @param cnt  calcule au fure et a mesure le nombre d'elements de la liste
    //             cree. l'effet de la fonction doit etre d'ajouter le nombre
    //             d'elements du sous-arbre de racine tree. Cependant, vous
    //             avez uniquement le droit d'utiliser l'opérateur ++.
    //
    static void linearize(Node *tree, Node *&list, size_t &cnt) noexcept
    {
        /* ... */
    }

public:
    //
    // @brief equilibre l'arbre
    //
    // applique l'algorithme d'equilibrage de l'arbre par linearisation et
    // arborisation
    //
    // Ne pas modifier cette fonction.
    //
    void balance() noexcept
    {
        size_t cnt = 0;
        Node *list = nullptr;
        linearize(_root, list, cnt);
        arborize(_root, list, cnt);
    }

private:
    //
    // @brief arborise les cnt premiers elements d'une liste en un arbre
    //
    // @param tree reference dans laquelle il faut ecrire la racine de l'arbre
    //             arborise par la fonction
    // @param list IN - reference a la tete de la liste a parcourir. La liste
    //                  est composee de Node dont le pointer left est nullptr
    //             OUT - debut de la suite de la liste dont on a utilise cnt
    //                   elements
    // @param cnt  nombre d'elements de la liste que l'on doit utiliser pour
    //             arboriser le sous arbre
    //
    static void arborize(Node *&tree, Node *&list, size_t cnt) noexcept
    {
    }

public:
    //
    // @brief Parcours pre-ordonne de l'arbre
    //
    // @param f une fonction capable d'être appelée en recevant une cle
    //          en parametre. Pour le noeud n courrant, l'appel sera
    //          f(n->key);
    //
    template <typename Fn>
    void visitPre(Fn f)
    {
        visitPre(_root, f);
    }
private:
    template <typename Fn>
    void visitPre(Node *r, Fn f)
    {
        if (r != nullptr)
        {
            f(r->key);
            visitPre(r->left, f);
            visitPre(r->right, f);
        }
    }
public:
    //
    // @brief Parcours symétrique de l'arbre
    //
    // @param f une fonction capable d'être appelée en recevant une cle
    //          en parametre. Pour le noeud n courrant, l'appel sera
    //          f(n->key);
    //
    template <typename Fn>
    void visitSym(Fn f)
    {
        visitSym(_root, f);
    }
private:
    template <typename Fn>
    void visitSym(Node *r, Fn f)
    {
        if (r != nullptr)
        {
            visitSym(r->left, f);
            f(r->key);
            visitSym(r->right, f);
        }
    }

public:
    //
    // @brief Parcours post-ordonne de l'arbre
    //
    // @param f une fonction capable d'être appelée en recevant une cle
    //          en parametre. Pour le noeud n courrant, l'appel sera
    //          f(n->key);
    //
    template <typename Fn>
    void visitPost(Fn f)
    {
        visitPost(_root, f);
    }

private:
    template <typename Fn>
    void visitPost(Node *r, Fn f)
    {
        if (r != nullptr)
        {
            visitPost(r->left, f);
            visitPost(r->right, f);
            f(r->key);
        }
    }
public:
    //
    // Les fonctions suivantes sont fournies pour permettre de tester votre classe
    // Merci de ne rien modifier au dela de cette ligne
    //
    void display() const
    {
        stringstream ss1, ss2, ss3;
        displayKeys(ss1);
        displayKeys(ss3);
        displayCounts(ss2);
        string l1, l2;

        size_t W = 11;
        while (getline(ss3, l1))
            if (l1.length() > W)
                W = l1.length();

        displayKeys(ss1);

        cout << "\n";
        cout << "+-" << left << setfill('-') << setw(W) << "-"
             << "+-" << setw(W) << "-" << setfill(' ') << "+" << endl;
        cout << "| " << left << setw(W) << "key"
             << "| " << setw(W) << "nbElements"
             << "|" << endl;
        cout << "+-" << left << setfill('-') << setw(W) << "-"
             << "+-" << setw(W) << "-" << setfill(' ') << "+" << endl;
        while (getline(ss1, l1) and getline(ss2, l2))
        {
            cout << "| " << left << setw(W) << l1 << "| " << setw(W) << l2 << "|" << endl;
        }
        cout << "+-" << left << setfill('-') << setw(W) << "-"
             << "+-" << setw(W) << "-" << setfill(' ') << "+" << endl;
    }

    void displayKeys(ostream &os = cout) const
    {
        display([](Node *n) -> const_reference { return n->key; }, os);
    }

    void displayCounts(ostream &os = cout) const
    {
        display([](Node *n) -> size_t { return n->nbElements; }, os);
    }

    //
    // utilise un parcours de l'arbre en largeur avec
    // un noeud sentinelle newLevel qui traque les
    // changements de niveaux
    //
    template <typename Fn>
    void display(Fn func, ostream &os = cout) const
    {
        Node *newLevel = (Node *)-1;
        // addresse non nulle dont on est sur qu'elle ne contient pas
        // vraiment un Node. Utilisée comme sentinelle.

        queue<Node *> Q;
        Q.push(_root);
        Q.push(newLevel);

        while (!Q.empty())
        {
            Node *cur = Q.front();
            Q.pop();

            if (cur == newLevel)
            {
                os << endl;
                if (!Q.empty())
                    Q.push(newLevel);
            }
            else if (cur == nullptr)
            {
                os << "- ";
            }
            else
            {
                os << func(cur) << " ";
                Q.push(cur->left);
                Q.push(cur->right);
            }
        }
    }
};