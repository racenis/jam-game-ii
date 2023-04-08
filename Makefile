src_camera.o: ./src/camera.cpp
	g++ -c -g -O0 -std=c++20 -I./src -I./../tram-sdk/src -I./../tram-sdk/libraries ./src/camera.cpp -o src_camera.o

src_levelswitch.o: ./src/levelswitch.cpp
	g++ -c -g -O0 -std=c++20 -I./src -I./../tram-sdk/src -I./../tram-sdk/libraries ./src/levelswitch.cpp -o src_levelswitch.o

src_main.o: ./src/main.cpp
	g++ -c -g -O0 -std=c++20 -I./src -I./../tram-sdk/src -I./../tram-sdk/libraries ./src/main.cpp -o src_main.o

src_mongus.o: ./src/mongus.cpp
	g++ -c -g -O0 -std=c++20 -I./src -I./../tram-sdk/src -I./../tram-sdk/libraries ./src/mongus.cpp -o src_mongus.o

src_score.o: ./src/score.cpp
	g++ -c -g -O0 -std=c++20 -I./src -I./../tram-sdk/src -I./../tram-sdk/libraries ./src/score.cpp -o src_score.o

src_entities_camera.o: ./src/entities/camera.cpp
	g++ -c -g -O0 -std=c++20 -I./src -I./../tram-sdk/src -I./../tram-sdk/libraries ./src/entities/camera.cpp -o src_entities_camera.o

src_entities_crab.o: ./src/entities/crab.cpp
	g++ -c -g -O0 -std=c++20 -I./src -I./../tram-sdk/src -I./../tram-sdk/libraries ./src/entities/crab.cpp -o src_entities_crab.o

src_entities_door.o: ./src/entities/door.cpp
	g++ -c -g -O0 -std=c++20 -I./src -I./../tram-sdk/src -I./../tram-sdk/libraries ./src/entities/door.cpp -o src_entities_door.o

src_entities_frog.o: ./src/entities/frog.cpp
	g++ -c -g -O0 -std=c++20 -I./src -I./../tram-sdk/src -I./../tram-sdk/libraries ./src/entities/frog.cpp -o src_entities_frog.o

src_entities_pickup.o: ./src/entities/pickup.cpp
	g++ -c -g -O0 -std=c++20 -I./src -I./../tram-sdk/src -I./../tram-sdk/libraries ./src/entities/pickup.cpp -o src_entities_pickup.o

src_entities_switch.o: ./src/entities/switch.cpp
	g++ -c -g -O0 -std=c++20 -I./src -I./../tram-sdk/src -I./../tram-sdk/libraries ./src/entities/switch.cpp -o src_entities_switch.o

src_entities_trigger.o: ./src/entities/trigger.cpp
	g++ -c -g -O0 -std=c++20 -I./src -I./../tram-sdk/src -I./../tram-sdk/libraries ./src/entities/trigger.cpp -o src_entities_trigger.o

clean:
	del src_camera.o
	del src_levelswitch.o
	del src_main.o
	del src_mongus.o
	del src_score.o
	del src_entities_camera.o
	del src_entities_crab.o
	del src_entities_door.o
	del src_entities_frog.o
	del src_entities_pickup.o
	del src_entities_switch.o
	del src_entities_trigger.o

project: src_camera.o src_levelswitch.o src_main.o src_mongus.o src_score.o src_entities_camera.o src_entities_crab.o src_entities_door.o src_entities_frog.o src_entities_pickup.o src_entities_switch.o src_entities_trigger.o 
	g++ -o sulasglaaze -L./../tram-sdk/libraries/binaries/win64/  -L./../tram-sdk -static src_camera.o src_levelswitch.o src_main.o src_mongus.o src_score.o src_entities_camera.o src_entities_crab.o src_entities_door.o src_entities_frog.o src_entities_pickup.o src_entities_switch.o src_entities_trigger.o -ltramsdk -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath -lglfw3 -lOpenAL32 -lgdi32 -lopengl32
