//  Binary Search Tree
//
//  Copyright (c) 2017 Olivier Cuisenaire. All rights reserved.

#include <iostream>
#include <algorithm>
#include "abr.cpp"
using namespace std;


class Int {
  int val;
public:
  static int timeBomb;
  Int() : val(0) {
  }
  Int(int i) : val(i) {
  }
  Int(const Int& i) : val(i.val) {
    if (++timeBomb == 0) throw std::logic_error("Int::CopyConstructor");
  }
  Int& operator=(const Int& i) {
    if (++timeBomb == 0) throw std::logic_error("Int::Operator=");
    val = i.val;
    return *this;
  }
  bool operator == ( const Int& i ) const noexcept {
    return val == i.val;
  }
  bool operator != ( const Int& i ) const noexcept {
    return not (*this == i);
  }
  bool operator < ( const Int& i ) const noexcept {
    return val < i.val;
  }
  bool operator > ( const Int& i ) const noexcept {
    return i < *this;
  }
  bool operator <= ( const Int& i ) const noexcept {
    return not (i < *this);
  }
  bool operator >= ( const Int& i ) const noexcept {
    return not (i > *this);
  }
  friend ostream& operator<< ( ostream& os, const Int& i );
};

ostream& operator<< ( ostream& os, const Int& i ) {
  os << i.val;
  return os;
}

int Int::timeBomb = 0;

void visitor(const Int& a) { cout << a << " "; }

int main() {

  try {

    vector<int> values = { 10, 12, 16, 15, 7, 1, 12, 5, 11, 11, 4, 2, 6, 0, 13 };

    BinarySearchTree<Int> abr;

    // **** INSERT ****

    cout << "Insertion de " << values.size() << " éléments\n";
    for(int i : values) {
      cout << i << " ";
      abr.insert(i);
    }
    cout << "\n";
    abr.display();

    // ***** CONTAINS ****

    const int M = *max_element(values.begin(), values.end());
    cout << "\nRecherche des valeurs de 0 à " << M << "\n";
    cout << "Trouves: ";
    for(int i = 0; i < M; ++i) {
      if(abr.contains(i))
        cout << i << " ";
    }
    cout << "\nAbsents: ";
    for(int i = 0; i < M; ++i) {
      if(not abr.contains(i))
        cout << i << " ";
    }
    cout << "\n\n";

    // ***** MIN ****

    cout << "Recherche du minimum: " << abr.min() << "\n\n";

    // ***** DELETE_MIN ****

    cout << "Suppression du minimum (2x) \n";
    for(int i = 0; i < 2; i++) {
      abr.deleteMin();
      abr.display();
      cout << "\n";
    }

    // ***** DELETE ****

    vector<int> toDelete = { 10, 16, 7, 10 };

    for(int i : toDelete ) {
      if(abr.deleteElement(i)) {
        cout << "Suppression de " << i << "\n";
        abr.display();
        cout << "\n";
      } else {
        cout << "Absence de " << i << ". Pas de suppression \n";
      }
    }

    // **** VISITES *****

    cout << "\nTest des parcours ... \n";
    cout << "Pre-ordonne : ";
    abr.visitPre(visitor);
    cout << "\n";

    cout << "Symetrique  : ";
    abr.visitSym(visitor);
    cout << "\n";

    cout << "Post-ordonne: ";
    abr.visitPost(visitor);
    cout << "\n\n";

    // **** SIZE ****

    cout << "Nombre d'éléments : " << abr.size() << "\n\n";

    // **** NTH_ELEMENT ****

    cout << "Test de nth_element \n";
    for(int i = 0; i < abr.size(); ++i) {
      cout << abr.nth_element(i) << " ";
    }
    cout << "\n\n";

    // **** RANK ****

    cout << "Test de rank: \n";
    for(int i = M; i >= 0; --i) {
      size_t r = abr.rank(i);
      if ( r != -1 )
        cout << r << " ";
    }
    cout << "\n\n";

    // **** CONSTRUCTEURS ET AFFECTATION ****

    cout << "Test du move constructor - abr2(move(abr)) \n";
    {
      BinarySearchTree<Int> abr2 ( move(abr) );
      cout << "abr2: "; abr2.display();
      cout << "\n";
      cout << "abr: "; abr.display();
      cout << "\n";

      {
        cout << "Test du copy constructor - abr3(abr2)\n";
        BinarySearchTree<Int> abr3 ( abr2 );
        cout << "\n";
        cout << "abr3: "; abr3.display();
        cout << "\n";

        {
          cout << "Test du move operator= - abr4 = move(abr2)\n";
          BinarySearchTree<Int> abr4;
          abr4 = move(abr2);
          cout << "abr4: "; abr4.display();
          cout << "\n";
          cout << "abr2: "; abr2.display();
          cout << "\n";
          cout << "Destruction abr4 \n";
        }
        cout << "\n\n";
        cout << "Test du copy operator= - abr2 = abr3 \n";
        abr2 = abr3;
        cout << "\n";
        cout << "abr2: "; abr2.display();
        cout << "\n";
        cout << "abr3: "; abr3.display();
        cout << "\n";

        cout << "Destruction abr3 \n";

      }
      cout << "\n\n";

      cout << "Test du swap - abr.swap(abr2) \n\n";

      abr.swap(abr2);
      cout << "abr: "; abr.display();
      cout << "\n";
      cout << "abr2: "; abr2.display();
      cout << "\n";

      cout << "Destruction abr2 \n";
    }
    cout << "\n";

    // **** GARANTIES FORTES A L'INSERTION ET A LA COPIE ****

    try {
      Int::timeBomb = -3;
      for(int i = 0; i < 10; ++i) {
        cout << "abr.insert(" << i << ") \n";
        abr.insert(i);
        cout << "Succes \n";
        cout << "abr: "; abr.display();
        cout << "\n";
      }
    } catch (...) {
      cout << "Exception capturée \n";
    }

    cout << "abr: "; abr.display();
    cout << "\n";

    try {
      Int::timeBomb = -9;
      cout << "Test de copie par constructeur manquee \n";
      BinarySearchTree<Int> abr2(abr);
    } catch (...) {
      cout << "\nException capturée \n\n";
    }


    {
      cout << "Creation abr2 \n";
      BinarySearchTree<Int> abr2;
      for(int i : { 3, 7, 4, 1, 2 } )
        abr2.insert(i);

      abr2.display();

    try {
      Int::timeBomb = -8;
      cout << "\nTest de copie par affectation manquee \n";
      abr2 = abr;
    } catch (...) {
      cout << "\nException capturée \n\n";
    }

    cout << "abr2: "; abr2.display();
    cout << "\n";

      // **** LINEARIZE ****

      cout << "\nTest de linearisation abr2 ";
      abr2.linearize();
      abr2.display();

      cout << "\nDestruction abr2 \n";

    }

    // **** BALANCE (donc ARBORIZE) ****

    cout << "\n\nTest d'equilibrage abr\n";
    abr.balance();
    abr.display();

    cout << "\nInsertion d'éléments 20, 21, 23, 22, 24 \n";
    for(int i : { 20, 21, 23, 22, 24 } )
      abr.insert(i);

    cout << endl;
    abr.display();

    cout << "\nTest d'equilibrage abr\n";
    abr.balance();
    abr.display();


    cout << "\nDestruction abr \n";

  } catch (...) {
    cout << "\nErreur - une exception imprévue a été capturée \n";
  }
  return 0;
}
