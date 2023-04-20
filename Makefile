src_camera.o: ./src/camera.cpp
	emcc -c -g -O0 -std=c++20 -I./src -Wno-undefined-var-template -I./../tram-sdk/src -I./../tram-sdk/libraries -I./../tram-sdk/src -I./../tram-sdk/libraries/bullet ./src/camera.cpp -o src_camera.o

src_levelswitch.o: ./src/levelswitch.cpp
	emcc -c -g -O0 -std=c++20 -I./src -Wno-undefined-var-template -I./../tram-sdk/src -I./../tram-sdk/libraries -I./../tram-sdk/src -I./../tram-sdk/libraries/bullet ./src/levelswitch.cpp -o src_levelswitch.o

src_main.o: ./src/main.cpp
	emcc -c -g -O0 -std=c++20 -I./src -Wno-undefined-var-template -I./../tram-sdk/src -I./../tram-sdk/libraries -I./../tram-sdk/src -I./../tram-sdk/libraries/bullet ./src/main.cpp -o src_main.o

src_mongus.o: ./src/mongus.cpp
	emcc -c -g -O0 -std=c++20 -I./src -Wno-undefined-var-template -I./../tram-sdk/src -I./../tram-sdk/libraries -I./../tram-sdk/src -I./../tram-sdk/libraries/bullet ./src/mongus.cpp -o src_mongus.o

src_score.o: ./src/score.cpp
	emcc -c -g -O0 -std=c++20 -I./src -Wno-undefined-var-template -I./../tram-sdk/src -I./../tram-sdk/libraries -I./../tram-sdk/src -I./../tram-sdk/libraries/bullet ./src/score.cpp -o src_score.o

src_sounds.o: ./src/sounds.cpp
	emcc -c -g -O0 -std=c++20 -I./src -Wno-undefined-var-template -I./../tram-sdk/src -I./../tram-sdk/libraries -I./../tram-sdk/src -I./../tram-sdk/libraries/bullet ./src/sounds.cpp -o src_sounds.o

src_entities_camera.o: ./src/entities/camera.cpp
	emcc -c -g -O0 -std=c++20 -I./src -Wno-undefined-var-template -I./../tram-sdk/src -I./../tram-sdk/libraries -I./../tram-sdk/src -I./../tram-sdk/libraries/bullet ./src/entities/camera.cpp -o src_entities_camera.o

src_entities_crab.o: ./src/entities/crab.cpp
	emcc -c -g -O0 -std=c++20 -I./src -Wno-undefined-var-template -I./../tram-sdk/src -I./../tram-sdk/libraries -I./../tram-sdk/src -I./../tram-sdk/libraries/bullet ./src/entities/crab.cpp -o src_entities_crab.o

src_entities_door.o: ./src/entities/door.cpp
	emcc -c -g -O0 -std=c++20 -I./src -Wno-undefined-var-template -I./../tram-sdk/src -I./../tram-sdk/libraries -I./../tram-sdk/src -I./../tram-sdk/libraries/bullet ./src/entities/door.cpp -o src_entities_door.o

src_entities_frog.o: ./src/entities/frog.cpp
	emcc -c -g -O0 -std=c++20 -I./src -Wno-undefined-var-template -I./../tram-sdk/src -I./../tram-sdk/libraries -I./../tram-sdk/src -I./../tram-sdk/libraries/bullet ./src/entities/frog.cpp -o src_entities_frog.o

src_entities_pickup.o: ./src/entities/pickup.cpp
	emcc -c -g -O0 -std=c++20 -I./src -Wno-undefined-var-template -I./../tram-sdk/src -I./../tram-sdk/libraries -I./../tram-sdk/src -I./../tram-sdk/libraries/bullet ./src/entities/pickup.cpp -o src_entities_pickup.o

src_entities_switch.o: ./src/entities/switch.cpp
	emcc -c -g -O0 -std=c++20 -I./src -Wno-undefined-var-template -I./../tram-sdk/src -I./../tram-sdk/libraries -I./../tram-sdk/src -I./../tram-sdk/libraries/bullet ./src/entities/switch.cpp -o src_entities_switch.o

src_entities_trigger.o: ./src/entities/trigger.cpp
	emcc -c -g -O0 -std=c++20 -I./src -Wno-undefined-var-template -I./../tram-sdk/src -I./../tram-sdk/libraries -I./../tram-sdk/src -I./../tram-sdk/libraries/bullet ./src/entities/trigger.cpp -o src_entities_trigger.o

clean:
	del src_camera.o
	del src_levelswitch.o
	del src_main.o
	del src_mongus.o
	del src_score.o
	del src_sounds.o
	del src_entities_camera.o
	del src_entities_crab.o
	del src_entities_door.o
	del src_entities_frog.o
	del src_entities_pickup.o
	del src_entities_switch.o
	del src_entities_trigger.o

project: src_camera.o src_levelswitch.o src_main.o src_mongus.o src_score.o src_sounds.o src_entities_camera.o src_entities_crab.o src_entities_door.o src_entities_frog.o src_entities_pickup.o src_entities_switch.o src_entities_trigger.o 
	emcc -g src_camera.o src_levelswitch.o src_main.o src_mongus.o src_score.o src_sounds.o src_entities_camera.o src_entities_crab.o src_entities_door.o src_entities_frog.o src_entities_pickup.o src_entities_switch.o src_entities_trigger.o -sASSERTIONS=2 -sSAFE_HEAP=0 -sALLOW_MEMORY_GROWTH -sSTACK_OVERFLOW_CHECK=1 -sUSE_BULLET=1 -sUSE_GLFW=3 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -L./  -L./../tram-sdk/libraries/binaries/web/ -L./../tram-sdk/ -ltramsdk -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath -o sulasglaaze.html --preload-file ./