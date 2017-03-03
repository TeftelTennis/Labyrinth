#ifndef NETWORK_H
#define NETWORK_H

/*
class ZALOOPA
{
public:
    /* Скрипт отвечает за любые сообщения между сервером и клентами
     *
     * Сообщение о ходе передаются в строке:
     * <type> [<direction>] [<item>]
     *
     * Сообщение о результате хода:
     * if move
     * 		<result> [<cnt> {<corpse>} <cnt> <trapName> <isTreasure>] (только если совершен ход)
     * 		<result> = "move" | "wall" // TODO : ESCAPE use DOOR
     * if shoot
     * 		<cnt> {<name>} <isSamePos> // isSamePos - если убийца тут же подбирает трупы.
     * if dig
     * 		<cnt> {<isTrap> <name>}
     * 		<isTrap> = "1" | "0" // if 1 == trap then 0 = item
     *

     //TODO:
     //		Коменты!!!
     // 	В начеле хода предупреждение о мягком месте
     //		Настройки Сервера(almost finished)

    //#pragma strict
    string repeatNameMessage = "INCORRECT NAME : This name is already in use on the server.";

    string playerName = "NooB";
    int maxResultSize = 400;
    // Типо указатели на обьекты.
    //var serverPrefab : GameObject;
    //var playerPrefab : GameObject;
    ServerData serverData;
    LabyrinthObjectFactory objectFactory;

    GameLog myLog;
    int W;
    int H;
    int startAmmo;
    int iStart;
    int jStart;
    int startLife;
    bool isServer = false;

    string map;
    bool myTurn;

    bool wait = false;

    function Start () {
        //DontDestroyOnLoad(gameObject);
        //serverData = GameObject.Find("Server Data").GetComponent(ServerData);
        //objectFactory = GameObject.Find("ObjectFactory").GetComponent(LabyrinthObjectFactory);
    }

    void createPlayer(string name, int w,int  h, int i, int j) {
        //Debug.Log("createPlayer : " + name);
        myLog = GameLog(w, h, startAmmo, startLife, true, i, j);
        playerName = name;
        NetworkPlayer player = networkView.owner;
        if (!isServer)
            networkView.RPC("addPlayer", RPCMode.Server, player, i, j, name);
        else
            addPlayer(player, i, j, name);
        networkView.RPC("addGameLog", RPCMode.AllBuffered, w, h, startAmmo, startLife, name, "All");
    }

    function launchServer(string nameP, int i, int j) {
        W = serverData.wigth;
        H = serverData.height;
        isServer = true;
        //Network.InitializeServer(32, 25000, false);
        //GameObject server = Instantiate(serverPrefab);
        //server.GetComponent(Server).initField();
        startAmmo = serverData.startAmmo;
        server.name = "Server";
        //Application.LoadLevel(serverData.map);
        createPlayer(nameP, W, H, i, j);
    }

    void connectToServer(string ip, int i, int j, string nameP) {
        //Network.Connect(ip, 25000);
        iStart = i;
        jStart = j;
        playerName = nameP;
        wait = true;
    }

    void Update() {
        if (wait) {
            //if (Network.peerType == NetworkPeerType.Client) {
            //    networkView.RPC("getData", RPCMode.Server, playerName);
            //    wait = false;
            //}
        }
        //Debug.Log(Network.peerType);
    }


    void sendTurn(string turn) {
        myTurn = false;
        if (!isServer) {
            //networkView.RPC("doTurn", RPCMode.Server, turn, playerName);
        }
        else {
            doTurn(turn, playerName);
        }
    }

    //****************************** THIS FUNCTIONS FOR SERVER:
    void sendWhoNext(string nameNext) {
        //networkView.RPC("setTurn", RPCMode.All, nameNext);
    }

    void sendResultOfTurn(string nameP, string turn, string result, string nameNext) {
        //networkView.RPC("doResultOfTurn", RPCMode.All, nameP, turn, result);
        //networkView.RPC("setTurn", RPCMode.All, nameNext);
    }

    //@RPC
    void doTurn(string turn, string nameP) {
//        GameObject.Find("Server").GetComponent(Server).doTurn(turn, nameP);
    }

    //@RPC
    void getData(string nameP) {
        //Debug.Log("HEAVY\\m/METALL");
        //var allObj : GameObject[] = Resources.FindObjectsOfTypeAll(typeof(GameObject));
        var correctName = true;
        //for (var i : int = 0; i < allObj.length; i++) {
        //   if (nameP == allObj[i].name) correctName = false;
        //}
        /*networkView.RPC("setAll",
            RPCMode.Others,
            serverData.weight,
            serverData.height,
            serverData.startAmmo,
            serverData.startLife,
            "Classic", correctName);
    }

    //@RPC
    void addPlayer(int i, int j, string nameP) {
        //Debug.Log("catched query to addPlayer : " + nameP);
        //Server.addPlayer(i, j, nameP, player);
    }

    //****************************** THIS FUNCTIONS FOR CLIENTS:
    //@RPC
    void doResultOfTurn(string nameP, string turnS, string resultS) {
        //Debug.Log(nameP + '|' + turnS + '|' + resultS);
        //var turn : String[] = turnS.Split([' '], 3, System.StringSplitOptions.None);
        //var result : String[] = resultS.Split([' '], maxResultSize, System.StringSplitOptions.None);
        //var gameLog : GameLog = GameObject.Find(nameP).GetComponent(PlayerGameLog).data;
        if (nameP == playerName) gameLog = myLog;
        if (turn[0] == "move") {
            if (result[0] == "wall") {
                gameLog.addWall(new Direction(turn[1]), "wall");
            } else if (result[0] == "move") {
                gameLog.addMove(new Direction(turn[1]));
                var corpseCount : int = int.Parse(result[1]);
                for (var it : int = 0; it < corpseCount; it++) {
                    var name : String = result[2 + it];
                    if (name != playerName) gameLog.player.take(GameObject.Find(name).GetComponent(PlayerGameLog).data.player);
                    else gameLog.player.take(myLog.player);
                }
                var trapCount : int = int.Parse(result[2 + corpseCount]);
                for (it = 0; it < trapCount; it++) {
                    name = result[3 + corpseCount + it];
                    var trap : Trap = objectFactory.createTrap(int.Parse(name));
                    trap.cought(gameLog.player, gameLog);
                }
                if (result[3 + corpseCount + trapCount] == "1") {
                    gameLog.addObject(new Treasure(new Item()));
                }
            }
        } else if (turn[0] == "shoot") {
            var tmpPlayer : Player = gameLog.player;
            var bullet : Item = objectFactory.createItem(int.Parse(turn[2]));
            tmpPlayer.deleteItem(bullet);

            var victimCount : int = int.Parse(result[0]);
            var isSamePos : boolean = result[victimCount + 1] == "1";
            for (it = 0; it < victimCount; it++) {
                name = result[1 + it];
                if (name != playerName) {
                    tmpPlayer = GameObject.Find(name).GetComponent(PlayerGameLog).data.player;
                } else {
                    tmpPlayer = myLog.player;
                }
                bullet.hitPlayer(tmpPlayer, gameLog);
                if (isSamePos)
                    gameLog.player.take(tmpPlayer);
            }
        } else if (turn[0] == "dig") {
            var count = int.Parse(result[0]);
            for (it = 0; it < count; it++) {
                var type : String = result[it * 2 + 1];
                name = result[it * 2 + 2];
                if (type == "0") {
                    gameLog.player.items.Add(objectFactory.createItem(int.Parse(name)));
                } else if (type == "1") {
                    objectFactory.createTrap(int.Parse(name)).cought(gameLog.player, gameLog);
                }
            }
        }
        //TODO: VISUALISE + Print messages
    }

    @RPC
    function setAll(w : int, h : int, startammo : int, startlife : int, MAP : String, correctName : boolean) {
        if (!correctName) {
            Network.Disconnect();
            //TODO:
                Debug.Log(repeatNameMessage);
            return;
        }
        startLife = startlife;
        W = w;
        H = h;
        map = MAP;
        startAmmo = startammo;
        createPlayer(playerName, w, h, iStart, jStart);
        Application.LoadLevel(map);
    }

    @RPC
    function addGameLog(w : int, h : int, ammo : int, life : int, name : String, toName : String) {
        Debug.Log(toName + " == " + playerName + " ---> " + name);
        if (toName != "All" && toName != playerName) {
            return;
        }
        var player : GameObject = Instantiate(playerPrefab, Vector3.zero, Quaternion(0, 0, 0, 0));
        player.GetComponent(PlayerGameLog).init(name, w * 2 - 1, h * 2 - 1, ammo, life, false, w - 1, h - 1);
        player.name = new String.Copy(name);
    }

    @RPC
    function setTurn(toName : String) {
        if (toName == playerName) {
            myTurn = true;
        }
    }
};*/

#endif // NETWORK_H
