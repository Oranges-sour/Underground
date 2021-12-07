#ifndef __GAME_SPRITE_TYPE_H__
#define __GAME_SPRITE_TYPE_H__

struct GameSpriteType {
    enum class Type0 {
        unknow,
        mapTile,   //��Ϸ��������ײ�ĵ�ͼ��
        hero,      //Ӣ��
        enemy,     //����
        item,      //��Ϸ��Ʒ
        bullet,    //�ӵ�
        building,  //����
        other      //����
    };

    enum class Type1 {
        unknow,
        heroBullet,  //Ӣ�۷������ӵ�
        enemyBullet  //���˷������ӵ�
    };

    bool operator<(const GameSpriteType& other) const
    {
        return (type0 < other.type0);
    }
    Type0 type0;
    Type1 type1;
};

#endif