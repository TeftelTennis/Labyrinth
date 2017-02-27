#ifndef LABYRINTH_H
#define LABYRINTH_H



class Direction {
public:
    int UP = 0;
    int LEFT = 1;
    int DOWN = 2;
    int RIGHT = 3;
    string name[4] = {"up", "left", "down", "right"};

    int dir;
    string getName();
    Direction();
    Direction(int a);
    Direction(string a);
    void turnClockwise();
    void turnCounterClockwise();
    void reverse();
};

class LabyrinthObject {
public:
    int TYPE_EMPTY = 0;
    int TYPE_ITEM = 1;
    int TYPE_PLAYER = 2;
    int TYPE_TRAP = 3;
    int TYPE_TREASURE = 4;

    int i, j, k;
    string name;
    int type;
    string tostring();
    LabyrinthObject ();
    function remove();
};

class Player : public LabyrinthObject {
public:
    vector<Item> items;
    int life;
    bool alive;

    int itemCount(int itemType);
    int itemCount(Item a);
    void deleteItem(int itemType);
    void deleteItem(Item a);
    Player(string Name, int Ammo, int playerLife);
    void take(Player corpse);
    int ammo();
};

class DSU {
private:
    int MAXSIZE = 1000;
public:
    int k;
    pair<int, int> parent[MAXSIZE][MAXSIZE];
    DSU(int w, int h);
    pair<int, int> color(int i, int j);
    void merge(int i1, int j1, int i2, int j2);
};

class LabyrinthData {
public:
    double wallProb;
    vector<Treasure> treasures;
    bool useRandomTreasure;
    int treasureCount;
    double loveToilets; // Если хочет ставить в сартир
    double staticTreasureProb;
    bool canPutTreasureTogether;
};

class Labyrinth {
private:
    int MAXSIZE = 1000;
public:
    int w;
    int h;
    vector<>[MAXSIZE][MAXSIZE] cell;               //<>TODODODO
    string horizontWalls[MAXSIZE][MAXSIZE];
    string verticalWalls[MAXSIZE][MAXSIZE];

    Labyrinth(int w, int h) {
        this->w = w;
        this->h = h;
        for (i = 0; i < h; i++) {
            for (j = 0; j < w + 1; j++) {
                verticalWalls[i, j] = "empty";
            }
        }
        for (i = 0; i < h + 1; i++) {
            for (j = 0; j < w; j++) {
                horizontWalls[i, j] = "empty";
            }
        }
    }

    bool checkPos(int i, int j) {
        return (i >= 0 && i < h && j >= 0 && j < w);
    }

    //По кордам и направлению дает новые корды.
    pair<int, int> move(int i, int j, Direction direct) {
        string direction = direct.getName();
        if (direction == "up") {
            i = i + 1;
        }
        if (direction == "down") {
            i = i - 1;
        }
        if (direction == "left") {
            j = j - 1;
        }
        if (direction == "right") {
            j = j + 1;
        }
        return make_pair(i, j);
    }

    string getWall(int i, int j, Direction direct) {
        string direction = direct.getName();
        if (direction == "up" || direction == "down") {
            if (direction == "up") {
                i++;
            }
            return horizontWalls[i, j];
        } else {
            if (direction == "right") {
                j++;
            }
            return verticalWalls[i, j];
        }
    }

    void addWall(int i, int j, Direction direct, string wall) {
        string direction = direct.getName();
        if (direction == "up" || direction == "down") {
            if (direction == "up") {
                i++;
            }
            horizontWalls[i, j] = wall;
        } else {
            if (direction == "right") {
                j++;
            }
            verticalWalls[i, j] = wall;
        }
    }

    tuple<int, int, int> findPlayer(string name) {
        Player player;
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                int it = 0;
                for (LabyrinthObject tmp : cell[i, j]) {
                    if (tmp.type == LabyrinthObject.TYPE_PLAYER) {
                        player = tmp;
                        if (player.name == name) {
                            return make_tuple(i, j, it);
                        }
                    }
                    it++;
                }
            }
        }
        return make_tuple(w + 1, h + 1, 1);
    }

    void movePlayer(string name, Direction direct) {
        string direction = direct.getName();
        Player player;
        tuple<int, int, int> pos = findPlayer(name);

        player = cell[pos.get(0), pos.get(1)][pos.get(2)];
        cell[pos.get(0), pos.get(1)].RemoveAt(pos.get(2));
        pair<int, int> tmp  = Labyrinth.move(pos.get(0), pos.get(1), direct);
        int i = tmp.first;
        int j = tmp.second;
        cell[i, j].push_back(player);
        player.i = i;
        player.j = j;
        player.k = cell[i, j].size() - 1;
    }

    void killPlayer(string name) {
        Debug.Log("Try to kill : " + name);
        tuple<int, int, int> playerPos = findPlayer(name);
        Player player = cell[playerPos.get(0), playerPos.get(1)][playerPos.get(2)];
        player.alive = false;
    }

    void addObject(int i, int j, LabyrinthObject item) {
        item.i = i;
        item.j = j;
        item.k = cell[i, j].size();
        cell[i, j].push_back(item);
    }

    void makeBorder() {
        for (int r = 0; r < h; r++) {
            verticalWalls[r, 0] = "border";
            verticalWalls[r, w] = "border";
        }
        for (int c  = 0; c < w; c++) {
            horizontWalls[0, c] = "border";
            horizontWalls[h, c] = "border";
        }
    }


    private var wasWall : boolean[,,];

    //settings:
    var data : LabyrinthData;

    function create() {
        wasWall = new boolean[h, w, 4];
        var dsu = new DSU(w, h);

        //Строит рандомный остов.
        while (dsu.k > 1) {
            var int i = Mathf.RoundToInt(Random.Range(-0.5 + float.Epsilon, h - 0.5 - float.Epsilon));
            var int j = Mathf.RoundToInt(Random.Range(-0.5 + float.Epsilon, w - 0.5 - float.Epsilon));
            var k : int = Mathf.RoundToInt(Random.Range(-0.5 + float.Epsilon, 4 - 0.5 - float.Epsilon));
            var newPos : Vector2 = move(i, j, new Direction(k));
            if (checkPos(newPos.get(0), newPos.get(1))) {
                var lastk : int = dsu.k;
                dsu.merge(i, j, newPos.get(0), newPos.get(1));
                if (lastk > dsu.k) {
                    wasWall[i, j, k] = true;
                    wasWall[newPos.get(0), newPos.get(1), (k + 2) % 4] = true;
                }
            }
        }

        //Ставит  остальные стены
        for (i = 0; i < h; i++) {
            for (j = 0; j < w; j++) {
                for (k = 0; k < 4; k++) {
                    if (!wasWall[i, j, k]) {
                        if (Random.value < data.wallProb) addWall(i, j, new Direction(k), "wall");
                    }
                }
            }
        }

        var pos : int = 0;

        //Put treasures
        for (i = 0; i <= data.treasureCount; i++) {
            var treasure : Treasure;
            if (i == 0)
                treasure = new Treasure(new Key());
            else
                if (data.useRandomTreasure) {
                    treasure = data.treasures[Mathf.FloorToInt(Random.Range(0, data.treasures.Count - float.Epsilon))];
                } else {
                    treasure = data.treasures[pos++];
                }
            var treasurePos : Vector2;
            var seed : int = 0; // How many times you tried to choose
            //Choose pos:
            while (true) {
                treasurePos.get(0) = Mathf.FloorToInt(Random.Range(0, h - float.Epsilon));
                treasurePos.get(1) = Mathf.FloorToInt(Random.Range(0, w - float.Epsilon));
                var wallCount : int = 0;
                for (j = 0; j < 4; j++) {
                    if (getWall(treasurePos.get(0), treasurePos.get(1), new Direction(j)) != "empty")
                        wallCount++;
                }
                var f : boolean = true;
                //check if other treasures here:
                if (!data.canPutTreasureTogether) {
                    for (var obj : LabyrinthObject in cell[treasurePos.get(0), treasurePos.get(1)]) {
                        if (obj.type == LabyrinthObject.TYPE_TREASURE) {
                            f = false;
                        }
                    }
                }
                if (!f) {
                    seed++;
                    continue;
                }

                var prob : float = wallCount * data.loveToilets + Mathf.Pow(1.1, seed) - 1 + data.staticTreasureProb;
                if (Random.value < prob) {
                    addObject(treasurePos.get(0), treasurePos.get(1), treasure);
                    break;
                }
                seed++;
            }
        }

        makeBorder();
    }
}

class GameLog extends Labyrinth {
    var previousVersion : GameLog;//Если ловушка переместила тебя.
    var player : Player;

    var turn : ArrayList;
    var iStart : int;
    var jStart : int;
    var iCur : int;
    var jCur : int;
    function addObject(a : Object) {
        cell[iCur, jCur].Add(a);
    }

    function GameLog(w : int, h : int, ammo : int, life : int, border : boolean, int i, int j) {
        player = new Player("HERO", ammo, life);
        super(w, h);
        turn = new ArrayList();
        iStart = i;
        jStart = j;
        iCur = i;
        jCur = j;
        if (border) {
            makeBorder();
        }
    }

    function addMove(Direction direct) {
        var direction : string = direct.getName();
        turn.Add(direction);
        var tmp : Vector2 = Labyrinth.move(iCur, jCur, direct);
        iCur = tmp.x;
        jCur = tmp.y;
    }

    function addWall(Direction direct, wall : string) {
        var direction : string = direct.getName();
        if (direction == "up" || direction == "down") {
            var int j = jCur;
            var int i = 0;
            if (direction == "down") {
                i = iCur;
            } else {
                i = iCur + 1;
            }
            horizontWalls[i, j] = wall;
        } else {
            i = iCur;
            j = 0;
            if (direction == "left") {
                j = jCur;
            } else {
                j = jCur + 1;
            }
            verticalWalls[i, j] = wall;
        }
    }
}

#endif // LABYRINTH_H
