BulletSpace.exe: main.cpp Ship.o Alien.o EnemyWave.o
	g++ main.cpp -o BulletSpace.exe -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system 

EnemyWave.o: EnemyWave.h Alien.o
	g++ -c EnemyWave.h

Alien.o: Alien.h GameObject.o
	g++ -c Alien.h 

Ship.o: Ship.h GameObject.o 
	g++ -c Ship.h

GameObject.o: GameObject.h Transform.o BulletPatterns.o Bullet.o
	g++ -c GameObject.h

Transform.o: Transform.h TransformStructs.o
	g++ -c Transform.h 

TransformStructs.o: TransformStructs.cpp
	g++ -c TransformStructs.cpp

BulletPatterns.o: BulletPattern.h  Bullet.o
	g++ -c BulletPattern.h

Bullet.o: Bullet.h
	g++ -c Bullet.h
clean:
	rm *.o assignment3