x = CreateEntity()
LoadTexture('assets/animationtest.bmp', 64, 15, true)
SetEntityTexture(x, 'assets/animationtest.bmp')
print("Hello world")
y = GetEntities()
print(#y)
MoveEntity(y[1], 100, 100)
DestroyEntity(y[2])
