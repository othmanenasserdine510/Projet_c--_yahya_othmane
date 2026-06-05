# 🎓 StudentManager — Système de Gestion des Étudiants en C++

> **Mini-Projet C++ · IMS 1ère Année · ESITH · Année Universitaire 2025/2026**

---

## 👥 Équipe

| Membre |
|--------|
| **AFIF Yahya** |
| **NASSERDINE Othmane** |

---

## 🔗 Liens importants

| Ressource | Lien |
|---|---|
| 📁 Dépôt GitHub | *https://github.com/sp7ce/Projet_c--_yahya_othmane* |
| 🎬 Vidéo de démonstration | *[à compléter]* |
| 📄 Rapport PDF | *[à compléter]* |

---

## 📌 Description du projet

**StudentManager** est un système complet de gestion des étudiants développé en **C++17 orienté objet**. Il couvre :

- Hiérarchie de classes avec **héritage et polymorphisme**
- Gestion mémoire via **smart pointers** (`std::shared_ptr`)
- Stockage et manipulation avec les **conteneurs et algorithmes STL**
- **Exceptions personnalisées** pour une robustesse totale
- **Persistance fichier texte** avec format `|` et fallback silencieux
- **Interface graphique Qt 6** complète avec filtrage, statistiques et mode sombre

---

## 🗂️ Structure du projet

```
StudentMgr/
├── src/
│   ├── models/
│   │   ├── Student.h                   # Classe abstraite de base
│   │   ├── UndergraduateStudent.h      # Étudiant Licence
│   │   ├── GraduateStudent.h           # Étudiant Master
│   │   └── PhDStudent.h               # Doctorant
│   ├── services/
│   │   └── StudentManager.h           # CRUD + tri + recherche (STL, header-only)
│   ├── exceptions/
│   │   └── StudentExceptions.hpp      # 4 exceptions personnalisées
│   ├── persistence/
│   │   └── PersistenceManager.h       # Sauvegarde/chargement texte (header-only)
│   └── gui/
│       ├── MainWindow.h/.cpp          # Fenêtre principale Qt
│       ├── StudentDialog.h/.cpp       # Formulaire ajout/modification
│       ├── StudentDetailsDialog.h     # Fiche détaillée
│       ├── StatsDialog.h              # Statistiques + histogramme QChart
│       └── CsvExporter.h             # Export CSV (header-only)
├── data/
│   └── students.txt                   # Données persistantes
├── tests/
│   └── test_main.cpp                  # Suite de tests complète
├── main.cpp                           # Point d'entrée console (tests parties 3 & 4)
├── main_gui.cpp                       # Point d'entrée interface graphique
├── StudentManagementApp.pro           # Fichier projet Qt (qmake)
└── README.md
```

---

## ⚙️ Prérequis

| Outil | Version minimale |
|---|---|
| Compilateur C++ | GCC ≥ 11 (C++17 requis) |
| Qt | Qt 6.x (`Widgets`, `Charts`) |
| qmake / Qt Creator | Qt Creator ≥ 9 |

---

## 🚀 Compilation et exécution

### Interface graphique (Qt Creator — recommandé)

```bash
git clone <url-du-depot>
cd StudentMgr
# Ouvrir StudentManagementApp.pro dans Qt Creator
# Build → Run
```

### Ligne de commande (qmake)

```bash
mkdir build && cd build
qmake ../StudentManagementApp.pro
make -j$(nproc)
./StudentManagementApp
```

### Mode console (parties 3 & 4)

```bash
g++ -std=c++17 -I. -o main_console main.cpp
./main_console
```

### Suite de tests (partie 6)

```bash
g++ -std=c++17 -I. -o tests_runner tests/test_main.cpp
./tests_runner
```

---

## 🧩 Hiérarchie de classes

```
Student  (classe abstraite)
│   Attributs protégés : name, id (≥1000), gpa ([0.0–4.0])
│   Méthodes virtuelles pures :
│       ├── virtual void display() const = 0
│       ├── virtual string getType() const = 0
│       └── virtual double computeScholarship() const = 0
│   Mutateurs avec validation :
│       ├── setName()  — lève std::invalid_argument si vide
│       └── setGpa()   — lève InvalidGradeException si hors [0,4]
│
├── UndergraduateStudent  [Licence]
│       Attribut : string major
│       computeScholarship() → 500 € si GPA ≥ 3.5, sinon 0 €
│
├── GraduateStudent  [Master]
│       Attribut : string researchTopic
│       computeScholarship() → GPA × 300 €
│
└── PhDStudent  [Doctorat]
        Attributs : string supervisor, int yearOfStudy
        computeScholarship() → 1 200 € (fixe)
```

---

## 📦 Fonctionnalités implémentées

### ✅ Partie 1 — Modélisation OOP
- Classe abstraite `Student` avec 3 méthodes virtuelles pures
- 3 classes concrètes avec attributs spécifiques et règles de bourse
- Validation dans les constructeurs (id ≥ 1000, gpa ∈ [0, 4])
- Polymorphisme complet sur `shared_ptr<Student>`

### ✅ Partie 2 — STL & Gestion des données

| Fonctionnalité | Implémentation STL |
|---|---|
| Stockage | `std::map<int, shared_ptr<Student>>` |
| Tri GPA décroissant | `std::sort` + lambda |
| Tri alphabétique | `std::sort` + `std::transform` + `tolower` |
| Recherche partielle | `string::find` en boucle range-based |
| Moyenne GPA | boucle range-based |
| Comptage par type | `std::count_if` |

### ✅ Partie 3 — Exceptions personnalisées

| Exception | Déclenchée quand |
|---|---|
| `InvalidIDException` | `id < 1000` dans le constructeur |
| `InvalidGradeException` | `gpa < 0` ou `gpa > 4` dans `setGpa()` / constructeur |
| `DuplicateIDException` | ID déjà présent dans `StudentManager::add()` |
| `StudentNotFoundException` | ID absent dans `findById()` / `remove()` |

### ✅ Partie 4 — Persistance

- Format `data/students.txt` avec séparateur `|`
- Chargement automatique au démarrage
- Sauvegarde après chaque opération CRUD
- Lignes corrompues ignorées silencieusement

### ✅ Partie 5 — Interface graphique Qt 6

**MainWindow** : QTableWidget, boutons CRUD, recherche temps réel, filtre par type, barre de statut  
**StudentDialog** : champs dynamiques selon le type, validation avec QMessageBox  
**StudentDetailsDialog** : fiche complète d'un étudiant  
**StatsDialog** : histogramme QChart des tranches de GPA  

### ✅ Partie 6 — Tests (37/37 ✓)

| Groupe | Tests |
|---|---|
| Types & polymorphisme | getType(), computeScholarship(), GPA limites |
| CRUD | add, remove, update, findById, findByName |
| Tri STL | GPA décroissant, alphabétique, countByType |
| Exceptions | 8 cas couverts |
| Persistance | save → reload → comparaison, fichier absent |

---

## 🎁 Bonus implémentés

| Bonus | Description |
|---|---|
| ✅ Export CSV | UTF-8 BOM, séparateur `;`, compatible Excel |
| ✅ Filtrage dynamique | Texte + type simultanément |
| ✅ Smart pointers exclusifs | Aucun pointeur brut |
| ✅ Histogramme QChart | Répartition des GPA par tranches |
| ✅ Mode sombre | Basculement clair ↔ sombre |

---

## 🌿 Branches Git suggérées

| Branche | Contenu |
|---|---|
| `main` | Version stable |
| `feature/oop` | Hiérarchie de classes |
| `feature/stl` | StudentManager |
| `feature/exceptions` | Exceptions personnalisées |
| `feature/persistence` | PersistenceManager |
| `feature/gui` | Interface Qt |
| `feature/tests` | Suite de tests |

---

## 📚 Références

- [Documentation Qt 6](https://doc.qt.io/qt-6/)
- [cppreference — STL Algorithms](https://en.cppreference.com/w/cpp/algorithm)
- [cppreference — shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr)

---

> 📅 Date de rendu : **15 mai 2026** · ESITH — École Supérieure des Industries du Textile et de l'Habillement
