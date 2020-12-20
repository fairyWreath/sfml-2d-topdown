#ifndef CATEGORY_HPP
#define CATEGORY_HPP


// dividing objects/entities to categories, each category receives similar command
namespace Category
{
	enum Type
	{
		None = 0,
		Scene = 1 << 0,
		PlayerCharacter = 1 << 1,
		SupportCharacter = 1 << 2,
		EnemyCharacter = 1 << 3,
		EnemyMobs = 1 << 4,

		AlliedProjectile = 1 << 6,
		EnemyProjectile = 1 << 7,

		SceneVoidLayer = 1 << 8,
	};
}



#endif