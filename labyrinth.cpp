#include "labyrinth.h"

#pragma strict
/*
 * типы стен :
 * 		wall
 * 		door
 * 		empty
 *
 * типы мягких мест :
 * 		item
 * 		ammo
 * 		trap
 * 		empty
 *
 * типы итемов :
 * 		bullet // не совсем итем
 * 		key
 *
 * типы ловушек :
 *
 */


// TODO: Return info from item;
// AddObject - Optimize

string Direction::getName() {
    return name[dir];
}


Direction::Direction() {
    dir = UP;
}

Direction::Direction(int a) {
    dir = a;
}

Direction::Direction(string a) {
    int c = 0;
    for (auto i : name) {
        if (i == a) {
            dir = c;
            return;
        }
        c++;
    }
}

void Direction::turnClockwise() {
    dir = (dir + 3) % 4;
}

void Direction::turnCounterClockwise() {
    dir = (dir + 1) % 4;
}

void Direction::reverse() {
    dir = (dir + 2) % 4;
}

LabyrinthObject::LabyrinthObject() {
    type = TYPE_EMPTY;
    name = "empty";
    tostring = []() {
        return name;
    };

}

function LabyrinthObject::remove() {
    type = TYPE_EMPTY;
    name = "empty";
    tostring = []() {
        return name;
    };
}

int Player::itemCount(int itemType) {
    int ans = 0;
    for (int i = 0; i < items.size(); i++) {
        Item tmp = items[i];
        if (tmp.itemType == itemType) {
            ans++;
        }
    }
    return ans;
}

int Player::itemCount(Item a) {
    return itemCount(a.itemType);
}

void Player::deleteItem(int itemType) {
    int ans = 0;
    for (int i = 0; i < items.size(); i++) {
        Item tmp = items[i];
        if (tmp.itemType == itemType) {
            items.RemoveAt(i);
            return;
        }
    }
}

void Player::deleteItem(Item a) {
    deleteItem(a.itemType);
}


Player::Player(string Name, int Ammo, int playerLife) {
    life = playerLife;
    type = TYPE_PLAYER;
    name = Name;
    alive = true;
    items.clear();
    for (int i = 0; i < Ammo; i++) {
        items.push_back(Bullet());
    }
    tostring = []() {
            string tmp = "player: name = " + name + "; life = " +
                life.Tostring() + "; alive = " + alive.Tostring()+ "; items : ";
            for (auto item : items) {
                tmp += '(' + item.tostring() + ' )';
            }
            return tmp + ";";
         };
}

void Player::take(Player corpse) {
    for (auto i : corpse.items) {
        items.push_back(i);
    }
    corpse.items.clear();
}

int Player::ammo() {
    return itemCount(Bullet());
}

DSU::DSU(int w, int h) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            parent[i, j] = make_pair(i, j);
        }
    }
    k = w * h;
}

pair<int, int> DSU::color(int i, int j) {
    pair<int, int> tmp;
    if (parent[i, j] == make_pair(i, j)) {
        return make_pair(i, j);
    } else {
        tmp = color(parent[i, j].first, parent[i, j].second);
        parent[i, j] = tmp;
        return tmp;
    }
}

void DSU::merge(int i1, int j1, int i2, int j2) {
    pair<int, int> a = color(i1, j1);
    pair<int, int> b = color(i2, j2);
    if (a == b) {
        return;
    }
    k--;
    parent[b.first, b.second] = a;
}



class Labyrinth {
    public var w : int;
    public var h : int;
    public var cell : ArrayList[,];
    public var horizontWalls : string[,];
    public var verticalWalls : string[,];

    function Labyrinth(W : int, H : int) {
        w = W;
        h = H;
        cell = new ArrayList[h, w];
        horizontWalls = new string[h + 1, w];
        verticalWalls = new string[h, w + 1];
        for (var i : int = 0; i < h; i++) {
            for (var j : int = 0; j < w; j++) {
                cell[i, j] = new ArrayList();
            }
        }
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

    private function checkPos(i : int, j : int) : boolean {
        return (i >= 0 && i < h && j >= 0 && j < w);
    }

    //По кордам и направлению дает новые корды.
    static function move(I : int, J : int, direct : Direction) : Vector2 {
        var i : int = I;
        var j : int = J;
        var direction : string = direct.getName();
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
        return Vector2(i, j);
    }

    function getWall(i : int, j : int, direct : Direction) {
        var direction : string = direct.getName();
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

    function addWall(i : int, j : int, direct : Direction, wall : string) {
        var direction : string = direct.getName();
        if (direction == "up" || direction == "down") {
            if (direction == "up") {
                i++;
            }
            horizontWalls[i, j] = wall;
        } else {
            if (direction == "right") {
                j++;
            }
            verticalWalls[i, j] = new string.Copy(wall);
        }
    }

    function findPlayer(name : string) : Vector3 {
        var player : Player;
        for (var i : int = 0; i < h; i++) {
            for (var j : int = 0; j < w; j++) {
                var it = 0;
                for (var tmp : LabyrinthObject in cell[i, j]) {
                    if (tmp.type == LabyrinthObject.TYPE_PLAYER) {
                        player = tmp;
                        if (player.name == name) {
                            return Vector3(i, j, it);
                        }
                    }
                    it++;
                }
            }
        }
        return Vector3(w + 1, h + 1, 1);
    }

    function movePlayer(name : string, direct : Direction) {
        var direction : string = direct.getName();
        var player : Player;
        var pos : Vector3 = findPlayer(name);

        player = cell[pos.x, pos.y][pos.z];
        cell[pos.x, pos.y].RemoveAt(pos.z);
        var tmp : Vector2 = Labyrinth.move(pos.x, pos.y, direct);
        var i : int = tmp.x;
        var j : int = tmp.y;
        cell[i, j].Add(player);
        player.i = i;
        player.j = j;
        player.k = cell[i, j].Count - 1;
    }

    function killPlayer(name : string) {
        Debug.Log("Try to kill : " + name);
        var playerPos : Vector3 = findPlayer(name);
        var player : Player = cell[playerPos.x, playerPos.y][playerPos.z];
        player.alive = false;
    }

    function addObject(i : int, j : int, item : LabyrinthObject) {
        item.i = i;
        item.j = j;
        item.k = cell[i, j].Count;
        cell[i, j].Add(item);
    }

    function makeBorder() {
        for (var r : int = 0; r < h; r++) {
            verticalWalls[r, 0] = new string.Copy("border");
            verticalWalls[r, w] = new string.Copy("border");
        }
        for (var c : int = 0; c < w; c++) {
            horizontWalls[0, c] = new string.Copy("border");
            horizontWalls[h, c] = new string.Copy("border");
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
            var i : int = Mathf.RoundToInt(Random.Range(-0.5 + float.Epsilon, h - 0.5 - float.Epsilon));
            var j : int = Mathf.RoundToInt(Random.Range(-0.5 + float.Epsilon, w - 0.5 - float.Epsilon));
            var k : int = Mathf.RoundToInt(Random.Range(-0.5 + float.Epsilon, 4 - 0.5 - float.Epsilon));
            var newPos : Vector2 = move(i, j, new Direction(k));
            if (checkPos(newPos.x, newPos.y)) {
                var lastk : int = dsu.k;
                dsu.merge(i, j, newPos.x, newPos.y);
                if (lastk > dsu.k) {
                    wasWall[i, j, k] = true;
                    wasWall[newPos.x, newPos.y, (k + 2) % 4] = true;
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
                treasurePos.x = Mathf.FloorToInt(Random.Range(0, h - float.Epsilon));
                treasurePos.y = Mathf.FloorToInt(Random.Range(0, w - float.Epsilon));
                var wallCount : int = 0;
                for (j = 0; j < 4; j++) {
                    if (getWall(treasurePos.x, treasurePos.y, new Direction(j)) != "empty")
                        wallCount++;
                }
                var f : boolean = true;
                //check if other treasures here:
                if (!data.canPutTreasureTogether) {
                    for (var obj : LabyrinthObject in cell[treasurePos.x, treasurePos.y]) {
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
                    addObject(treasurePos.x, treasurePos.y, treasure);
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

    function GameLog(w : int, h : int, ammo : int, life : int, border : boolean, i : int, j : int) {
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

    function addMove(direct : Direction) {
        var direction : string = direct.getName();
        turn.Add(direction);
        var tmp : Vector2 = Labyrinth.move(iCur, jCur, direct);
        iCur = tmp.x;
        jCur = tmp.y;
    }

    function addWall(direct : Direction, wall : string) {
        var direction : string = direct.getName();
        if (direction == "up" || direction == "down") {
            var j : int = jCur;
            var i : int = 0;
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
