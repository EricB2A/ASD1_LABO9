/*
 -----------------------------------------------------------------------------------
 Laboratoire : 09
 Fichier     : abr.cpp
 Auteur(s)   : Eric Bousbaa, Lucas Gianinetti, Cassandre Wojciechowski
 Date        : 13 juin 2019

 But         : Implémentation d'un arbre binaire de recherche.

 Compilateur : - MinGW-gcc 6.3.0
               - Apple LLVM version 9.0.0 (clang-900.0.39.2)

 Remarques   : Dans les compléxité, n fait référence au nombre de noeuds 
               présents dans l'arbre. 

 Copyright (c) 2017 Olivier Cuisenaire. All rights reserved.
 -----------------------------------------------------------------------------------
*/

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
     * Racine de l'arbre. Nullptr si l'arbre est vide.
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
     *  @remark Complexité : O(n).
     */
    BinarySearchTree(const BinarySearchTree &other)
    {
        if (other._root)
        {
            //On essaye d'effectuer la copie
            try{
                _root = new Node(other._root->key);
                copyTree(other._root, _root);
            }
            //Si la copie n'a pas reussi, 
            //on supprime ce qui a été créé pendant l'essai
            catch (...)
            {
                deleteSubTree(_root);
                _root = nullptr;
                throw;
            }
        }
    }

    /**
     *  @brief Opérateur d'affectation par copie.
     *
     *  @param other Le BinarySearchTree (BST) à copier
     * 
     *  @remark Complexié O(N) avec N le nombre de sous arbres 
     */
    BinarySearchTree &operator=(const BinarySearchTree &other)
    {
        Node* root = nullptr;
        if(other._root){
            root = new Node(other._root->key);
            try{
                copyTree(other._root,root);
            }catch(...){
                deleteSubTree(root);
                throw;
            }
        }
        _root = root;
        return *this;
    }

    /**
     *  @brief Echange le contenu avec un autre BST
     *
     *  @param other le BST avec lequel on echange le contenu
     *
     *  @remark Complexité O(1)
     */
    void swap(BinarySearchTree &other) noexcept
    {
        if(_root != other._root){
            Node *temp = _root;
            _root = other._root;
            other._root = temp;
        }
    }

    /**
     *  @brief constructeur de copie par déplacement
     *
     *  @param other le BST dont on vole le contenu
     *
     *  @remark Complexité O(1)
     */
    BinarySearchTree(BinarySearchTree &&other) noexcept
    {
        if(other._root){
            _root = other._root;
            other._root = nullptr;
        }else{
            _root = nullptr;
        }
    }

    /**
     *  @brief Opérateur d'affectation par déplacement.
     *
     *  @param other le BST dont on vole le contenu
     *
     *  @remark Complexité O(1)
     */
    BinarySearchTree &operator=(BinarySearchTree &&other) noexcept
    {
        if(_root != other._root){
            _root = other._root;
            other._root = nullptr;
        }else if(other._root){
            _root = nullptr;
        }
        return *this;
    }

    //
    // @brief Destructeur
    //
    // @remark Complexité O(N) avec N le nombre de noeuds dans l'arbre
    // 
    //
    ~BinarySearchTree()
    {
        deleteSubTree(_root);
    }

private:
    /**
     * @brief Copie un BTS dans un autre.
     *
     * @param src Racine du BTS a copier.
     * @param src Racine du BTS copié.
     *
     * @remark Complexité : O(n) avec n nombre de noeud dans src.
     */
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

     /**
     * @brief Fonction détruisant sous-arbre donné.
     *
     * @param r Racine du sous-arbre à détruire.
     *          Peut éventuellement être nul (nullptr).
     *
     * @remark Complexité : O(log(n))
     */
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
    // @remark Complexité O(log(N)) avec N le nombre de noeuds dans l'arbre
    //
    void insert(const_reference key)
    {
        insert(_root, key);
    }

private:
    /**
     * @brief Insertion d'une clef sans un sous-arbre.
     *
     * @param r Racine du sous-arbre dans lequel la clef est inséré.
     * @param key Clef a insérer.
     *
     * @return true si la clef est insérée. false si déjà présente.
     *
     * @remark Complexité : O(log(n))
     */
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
     /**
     * @brief Recherche d'une clef.
     *
     * @param key La clef a rechercher.
     *
     * @return true si clef trouvée, false dans le cas contraire.
     */
    bool contains(const_reference key) const noexcept
    {
        return contains(_root, key);
    }

private:
    /**
     * @brief
     *
     * @param key La clef à rechercher.
     * @param r La racine du sous-arbre.
     *
     * @return true sur la clef est trouvée, false sinon.
     *
     * @remark Complexité : O(log(n))
     */
    static bool contains(Node *r, const_reference key) noexcept {
        // Si la racine pointe sur null l'arbre est soit vide, soit on a pas trouvé la valeur
        if (r == nullptr){
            return false;
        }else if (key == r->key){
            return true;
        // Si la valeur est plus grande que la clé de la racine on va verifier dans le sous arbre droit
        }else if (key > r->key){
            contains(r->right, key);
        }else{ // Sinon dans le sous arbre gauche
            contains(r->left, key);
        }
    }
    
    /**
     * @brief Recherche le noeud minimum de l'ABR
     * 
     * @param r Noeud à partir duquel va s'effectuer la recherche.
     * 
     * @exception std::logic_error Si arbre vide.
     * 
     * @return Noeud minimum.
     * 
     * @remark Complexité : O(log(n))
     */
    static Node *minNode(Node *r){
        if(r == nullptr){
            throw std::logic_error("L'arbre est vide il n'y a donc pas de minimum");

        }
        else{
            if(r->left == nullptr){
                return r;
            }
            else{
                minNode(r->left);
            }
        }
    }

public:
    /**
     * @brief Rcherche de la clef minimale.
     *
     * @return référence constante vers le noeud minimal.
     */
    const_reference min() const{
        this->minNode(_root)->key;
    }

    /**
     * @brief Supprime le plus petit élément de l'arbre.
     *
     * @exception std::logic_error Si l'arbre est vide.
     *
     * @remark : Complexité : O(log(n))
     */
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
        delete noeud;
        delete temp;
    };

    /**
     * @brief Supprime l'élément de la clef de l'arbre.
     * 
     * @param key Clef de l'élément a supprimer
     * 
     * Ne pas modifier mais écrire la fonction
     * récursive privée deleteElement(Node*&,const_reference)
     */
    bool deleteElement(const_reference key) noexcept
    {
        return deleteElement(_root, key);
    }

private:
    /**
     * @brief Recherche le noeud minimal. Similaire à 
     * 
     * @param noeud
     * 
     * @return Renvoi une référence vers le noeud minimal de l'ABR.
     * 
     * @remark Complexité : O(log(n))
     */
    static Node *&findMinNode(Node *&r){
        if(r->left != nullptr){
            r->nbElements--;
            return findMinNode(r->left);
        }
        return r;
    }

    /**
     * @brief Supprime l'élément clef du sous-arbre.
     * 
     * @param r Racine du sous-arbre.
     * @param key Elément à supprimer.
     * 
     * @remark Complexité : O(log(n))
     */
    static bool deleteElement(Node *&r, const_reference key) noexcept {
        if (r == nullptr || !contains(r,key)){
            return false;
        }
        else if (key < r->key){
            if(deleteElement(r->left, key))
                r->nbElements--;
            return true;
        }
        else if (key > r->key){
            if(deleteElement(r->right, key))
                r->nbElements--;
            return true;
        }
        else if (r->key == key){
            Node *tmp = r;
            if (r->left == nullptr){
                std::swap(r, r->right);
                delete tmp;
                return true;
            }
            else if (r->right == nullptr){
                std::swap(r, r->left);
                delete tmp;
                return true;
            }
            else{
                Node *&min = findMinNode(r->right);
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
    /**
     * @brief Taille de l'arbre.
     * 
     * @return Le nombre d'élément de l'arbre.
     * 
     * @remark Complexité : O(1)
     */
    size_t size() const noexcept {
        return _root ? _root->nbElements : 0;
    }

    /**
     * @brief Clef en position n
     * 
     * @return Une référence à la clef en position n par ordre croissant des
     *         éléments.
     * 
     * @exception std::logic_error (std::out_of_range) Si nombre d'éléments dans 
     *            l'arbre inférieur à élément n.
     * 
     * @remark Complexité : O(log(n))
     */
    const_reference nth_element(size_t n) const {
        if(_root->nbElements < n){
            throw std::out_of_range("L'arbre ne contient pas autant d'elements");
        }
        return nth_element(_root, n);
    }

private:
    /**
     * @brief Clef en position n dans un sous-arbre.
     * 
     * @param r Racine du sous-arbre. Ne peut être nul (nullptr).
     * @param n Position n.
     * 
     * @ return Une référence à la clef en position n, par ordre croissant
     *          des éléments.
     * 
     * @remark Complexité : O(log(n))
     */
    static const_reference nth_element(Node *r, size_t n) noexcept {
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

    /**
     * @brief Position d'une clef dans l'ordre croissant des éléments de l'arbre.
     * 
     * @param key La clef dont on cherche le rang.
     * 
     * @return La position entre  0 et size()-1, 
     *         size_t(-1) si la clef est absente.
     * 
     */
    size_t rank(const_reference key) const noexcept {
        return rank(_root, key);
    }

private:
    /**
     * @brief Position d'une clef dans l'ordre croissant des éléments du sous-arbre.
     * 
     * @param key La clef dont on cherche le rang.
     * @param r Racine du sous-arbre.
     * 
     * @return La position entre  0 et size()-1, 
     *         size_t(-1) si la clef est absente.
     * 
     * @remark Complexité : O(log(n))
     */
    static size_t rank(Node *r, const_reference key) noexcept
    {
        if (contains(r, key) && r!= nullptr) {
            size_t nbElementsGauche = 0;

            if (r->left) {
                nbElementsGauche += r->left->nbElements;
            }

            if (key > r->key){
                return rank(r->right, key) + nbElementsGauche + 1;
            } else if (key < r->key){
                return rank(r->left, key);
            }else{
                return nbElementsGauche;
            }
        }
        else{
            return (size_t)-1;
        }
    }

public:
    /**
     * @brief Linéarise l'arbre
     * 
     * transforme l'arbre en une liste simplement chainee composee des memes
     * noeuds que precedemment, mais dont les pointeurs left sont tous egaux
     * a nullptr. Cette liste doit toujours respecter les conditions d'un
     * arbre binaire de recherche
     * 
     * Ne pas modifier cette fonction qui sert essentiellement a tester la
     * fonction recursive linearize(Node*, Node*&, size_t&) utilisée par
     * la methode publique arborize
     */
    void linearize() noexcept {
        size_t cnt = 0;
        Node *list = nullptr;
        linearize(_root, list, cnt);
        _root = list;
    }

private:
    /**
     * @brief Linéarise le sous-arbre donné en une liste.
     * 
     * @param tree Pointe vers la racine du sous-arbre à linéariser.
     * @param list Référence à la tête de la liste pour la créer. Sera modifié
     *             par la fonction afin que le début de la liste soit le plus
     *             petit élément de l'arbre.
     * @param cnt  Nombre d'éléments présents dans la liste.
     * 
     * @remark Complexité : O(n)
     */
    static void linearize(Node *tree, Node *&list, size_t &cnt) noexcept {
        if(tree){
            linearize(tree->right, list, cnt);
            tree->right = list;
            list = tree;
            list->nbElements = ++cnt;
            linearize(tree->left, list, cnt);
            tree->left = nullptr;
        }
    }

public:
    //
    // @brief equilibre l'arbre
    //
    // @remark Complexité O(N) avec N le nombre de noeuds dans l'arbre
    //
    // applique l'algorithme d'equilibrage de l'arbre par linearisation et
    // arborisation
    //
    // Ne pas modifier cette fonction.
    //

    /**
     * @brief Equilibrage de l'arbre.
     * 
     * applique l'algorithme d'equilibrage de l'arbre par linearisation et
     * arborisation
     * Ne pas modifier cette fonction
     */
    void balance() noexcept {
        size_t cnt = 0;
        Node *list = nullptr;
        linearize(_root, list, cnt);
        arborize(_root, list, cnt);
    }

private:
    /**
     * @brief Arborise les cnt premiers éléments d'une liste en un arbre.
     * 
     * @param tree Référence dans laquelle il faut écrire la racine de l'arbre
     *             arborisé par la fonction.
     * @param list IN - référence à la tête de la liste à parcourir. La liste
     *                  est composée de Node dont le pointer left est nullptr.
     *             OUT - début de la suite de la liste dont on a utilisé 
     *                   cnt éléments.
     * @param cnt Nombre d'éléments de la liste que l'on doit utiliser pour
     *            arboriser le sous-arbre.  
     * 
     * @remark Complexité : O(n)
     */
    static void arborize(Node *&tree, Node *&list, size_t cnt) noexcept {
        if(!cnt){
            tree = nullptr;
            return;
        }
        Node *rg = nullptr;
        arborize(rg, list, (cnt - 1) / 2);
        tree = list;
        tree->nbElements = cnt;
        tree->left = rg;
        list = list->right;
        arborize(tree->right, list, cnt / 2);
    }

public:
    //
    // @brief Parcours pre-ordonne de l'arbre
    //
    // @param f une fonction capable d'être appelée en recevant une cle
    //          en parametre. Pour le noeud n courrant, l'appel sera
    //          f(n->key);
    // @remark Complexité O(N) avec N le nombre de noeuds dans l'arbre
    //

    /**
     * @brief Parcours pré-ordonné de l'arbre.
     * 
     * @param f Une fonction capable d'être appelée en recevant une cle
     *          en parametre. Pour le noeud n courrant, l'appel sera f(n->key);
     * 
     * Complexité : O(n)
     */
    template <typename Fn>
    void visitPre(Fn f){
        visitPre(_root, f);
    }
private:
    template <typename Fn>
    void visitPre(Node *r, Fn f) {
        if (r != nullptr){
            f(r->key);
            visitPre(r->left, f);
            visitPre(r->right, f);
        }
    }
public:
    /**
     * @brief Parcours symétrique de l'arbre.
     * 
     * @param f Une fonction capable d'être appelée en recevant une cle
     *          en parametre. Pour le noeud n courrant, l'appel sera
     *          f(n->key);
     * 
     * @remark Complexité : O(n)
     */
    template <typename Fn>
    void visitSym(Fn f)
    {
        visitSym(_root, f);
    }

private:
    template <typename Fn>
    void visitSym(Node *r, Fn f)
    {
        if (r != nullptr){
            visitSym(r->left, f);
            f(r->key);
            visitSym(r->right, f);
        }
    }

public:
    /**
     * @brief Parcours post-ordonné de l'arbre.
     * 
     * @param f Une fonction capable d'être appelée en recevant une cle
     *         en parametre. Pour le noeud n courrant, l'appel sera
     *          f(n->key);
     * 
     * @remark Complexité : O(n)
     */
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