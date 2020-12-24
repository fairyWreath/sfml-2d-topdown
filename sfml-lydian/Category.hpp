#ifndef CATEGORY_HPP
#define CATEGORY_HPP


// dividing objects/entities to categories, each category receives similar command
// for scene nodes

namespace Category
{
	enum Type
	{
		None = 0,
		SceneVoidLayer = 1 << 0,
		PlayerCharacter = 1 << 1,
		SupportCharacter = 1 << 2,
		EnemyCharacter = 1 << 3,
		EnemyMobs = 1 << 4,

		Powerup = 1 << 5,
		AlliedProjectile = 1 << 6,
		EnemyProjectile = 1 << 7,

		// use bitwise left so types can be appended like this
		Character = PlayerCharacter | SupportCharacter | EnemyCharacter,
		Projectile = AlliedProjectile | EnemyProjectile,

		// entities =  for collision
		Entities = Character | Projectile,
	};
}



#endif