#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;
//Introduction
//Push Box with Monster
//
//U are zombie, push brain over the disk
//The Monster(Knight Indeed) move one step every two step you move,If Monster hit u, u die.U shall hit Monster first to kill it.
//Control:
//Up Down Left Right to Move
//s save
//l load
//pause/break  pause/resume the game
//r restart this stage
//
//materials download link:
//https://www.huiyi8.com/

//screen attributes
const int SCREEN_WIDTH=960;
const int SCREEN_HEIGHT=960;
const int SCREEN_BPP=32;

const int M=15;//number of rows of grid
const int N=15;

const int W=64;//width of every grid
const int H=64;

//SDL_Surface * background=NULL;
SDL_Surface * screen=NULL;
SDL_Surface * monster=NULL;
SDL_Surface * wall=NULL;
SDL_Surface * brain=NULL;
SDL_Surface * disk=NULL;
SDL_Surface * hero=NULL;
SDL_Surface * message=NULL;
SDL_Surface * failPic=NULL;
SDL_Surface * winPic=NULL;

TTF_Font *font = NULL;

const SDL_Color textColor = { 0x00, 0x00, 0x00  };

Mix_Music *music = NULL;
Mix_Chunk *movement=NULL;
Mix_Chunk *kill=NULL;
Mix_Chunk *failure=NULL;
Mix_Chunk *clearance=NULL;
Mix_Chunk *victory=NULL;


SDL_Event event;

// 0 Blank
// 1 Wall
// 2 Barin
// 3 Disk
// 4 Hero
// 5 Monster
const vector<vector<vector<int>>> mats=//0 for blank ,1 for wall
{//diaglo transpose
    //{
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },

    //},
    {
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0 },
        {  0,  0,  0,  0,  0,  0,  1,  3,  1,  0,  0,  0,  0,  0,  0 },
        {  0,  1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  0,  0,  0,  0 },
        {  0,  1,  4,  0,  0,  0,  0,  5,  2,  3,  1,  0,  0,  0,  0 },
        {  0,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  0,  0,  0,  0 },
        {  0,  0,  0,  0,  0,  0,  1,  2,  1,  0,  0,  0,  0,  0,  0 },
        {  0,  0,  0,  0,  0,  0,  1,  3,  1,  0,  0,  0,  0,  0,  0 },
        {  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0 },
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },

    },
    {
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0 },
        {  0,  0,  0,  0,  1,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0 },
        {  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0 },
        {  0,  1,  1,  1,  0,  2,  4,  0,  1,  1,  1,  0,  0,  0,  0 },
        {  0,  1,  3,  3,  2,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0 },
        {  0,  1,  1,  1,  0,  0,  0,  5,  0,  0,  1,  0,  0,  0,  0 },
        {  0,  0,  0,  1,  0,  2,  3,  0,  0,  0,  1,  0,  0,  0,  0 },
        {  0,  0,  0,  1,  2,  1,  0,  3,  2,  0,  1,  0,  0,  0,  0 },
        {  0,  0,  0,  1,  3,  1,  0,  0,  0,  1,  1,  0,  0,  0,  0 },
        {  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0 },
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },

    },
    //{
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0 },
        //{  0,  0,  0,  1,  0,  0,  3,  1,  0,  4,  1,  0,  0,  0,  0 },
        //{  0,  0,  0,  1,  0,  1,  0,  2,  0,  0,  1,  0,  0,  0,  0 },
        //{  0,  0,  0,  1,  0,  2,  3,  1,  2,  0,  1,  0,  0,  0,  0 },
        //{  0,  0,  0,  1,  1,  0,  3,  0,  5,  0,  1,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  1,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },

    //},
};
// 0 Blank
// 1 Wall
// 2 Barin
// 3 Disk
// 4 Hero
// 5 Monster
//

vector<vector<int>> mat;//注意mat用于存储状态,而mats则用来进行复原Lw
//The surfaces

ostream& operator<<(ostream& os,const vector<vector<int>>& mat)
{
    for(int i=0;i<mat.size();i++)
    {
        for(int j=0;j<mat[i].size();j++)
        {
            cout<<mat[i][j]<<' ';
        }
        cout<<endl;
    }
    return os;
}

class Brain{
    private:
        int x;
        int y;
    public:
        int GetX(){return x;}
        int GetY(){return y;}
        bool Show();
        bool MoveUp();
        bool MoveDown();
        bool MoveLeft();
        bool MoveRight();
        Brain(int x,int y):x(x),y(y){}

};

class Disk{
    private:
    int x;
    int y;
    public:
    int GetX(){return x;}
    int GetY(){return y;}
    bool Show();
    Disk(int x,int y):x(x),y(y){}
};

class Hero {
    private:
        int x;
        int y;
    public:
        int GetX(){return x;}
        int GetY(){return y;}
        bool Show();
        bool MoveUp();
        bool MoveDown();
        bool MoveLeft();
        bool MoveRight();
        Hero(int x,int y):x(x),y(y){}
};

class Monster {//Monster can only be over 0, can move one step every two steps the Hero move
//Get killed when hit by Hero
//Kill Hero when move to the same grid in the same ground
    private:
        int x;
        int y;
    public:
        int GetX(){return x;}
        int GetY(){return y;}
        Monster(int x,int y):x(x),y(y){}
        bool Move();
        bool Show();
};


vector<Brain*> Bs;
vector<Disk*> Ds;
Hero* Hr=NULL;
Monster* Ms=NULL;
int lvl;

class Map {
    private:

        //SDL_Surface* suf;
    public:
        bool Show();
        //Map(int lvl,SDL_Surface* suf):lvl(lvl),suf(suf){}
        Map(int l=-1)
    {
        if(l!=-1)
        {
            lvl=l;
            mat=mats[lvl];
        }
        char str[90];
        sprintf(str,"Push Box\tLevel:%d",lvl+1);
        SDL_WM_SetCaption(str, NULL     );

        for(int i=0;i<mat.size();i++)
        {
            for(int j=0;j<mat[i].size();j++)
            {
                switch(mat[i][j])
                {
                    case 2:
                        Bs.push_back(new Brain(i,j));
                        break;

                    case 3:
                        Ds.push_back(new Disk(i,j));
                        break;

                    case 4:
                        if(Hr==NULL)
                            Hr=new Hero(i,j);
                        break;

                    case 5:
                        if(Ms==NULL)
                            Ms=new Monster(i,j);
                        mat[i][j]=0;
                        break;
                }

            }
        }
    }
};

Map *Mp=NULL;// Map


SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            //SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0, 0) );
            //SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0xFF, 0xFF, 0xFF) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}
bool init()
{
    if(SDL_Init(SDL_INIT_EVERYTHING)==-1)
    {
        return false;
    }

    screen=SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,SDL_SWSURFACE);

    if(screen==NULL)
    {
        return false;
    }

    if(TTF_Init()==-1)
    {
        return false;
    }

    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096  ) == -1  )
    {
        return false;
    }

    SDL_WM_SetCaption( "Push Box", NULL );

    return true;
}
void clean_up()
{
    //Free the surfaces
    //SDL_FreeSurface( background );
    SDL_FreeSurface( monster );
    SDL_FreeSurface( wall );
    SDL_FreeSurface( brain );
    SDL_FreeSurface( disk );
    SDL_FreeSurface( hero );
    SDL_FreeSurface( failPic );
    SDL_FreeSurface( winPic );


    //Free the sound effects
    Mix_FreeChunk( movement );
    Mix_FreeChunk( kill );
    Mix_FreeChunk( failure );
    Mix_FreeChunk( clearance );
    Mix_FreeChunk( victory );


    //Free the music
    Mix_FreeMusic( music );

    //Close the font
    TTF_CloseFont( font );

    //Quit SDL_mixer
    Mix_CloseAudio();

    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}
bool load_files()
{
    //Load the background image
    //background = load_image( "Background.jpg"  );
    monster=load_image("Monster.jpg");
    wall=load_image("Wall.jpg");
    brain=load_image("Brain.jpg");
    disk=load_image("Disk.jpg");
    hero=load_image("Hero.jpg");
    failPic=load_image("fail.jpg");
    winPic=load_image("win.jpg");

    //Open the font
    font = TTF_OpenFont( "Ubuntu-B.ttf", 50  );

    //If there was a problem in loading the background
    if( monster==NULL||wall==NULL||brain==NULL||disk==NULL||hero==NULL||failPic==NULL||winPic==NULL )
    //if( background == NULL ||monster==NULL||wall==NULL||brain==NULL||disk==NULL||hero==NULL||failPic==NULL||winPic==NULL )
    {
        return false;
    }
    if(monster==NULL||wall==NULL||brain==NULL||disk==NULL||hero==NULL)
    {
        return false;
    }

    //If there was an error in loading the font
    if( font == NULL  )
    {
        return false;
    }

        //Load the music
    music = Mix_LoadMUS( "music.wav"  );

    movement=Mix_LoadWAV("move.wav");
    kill=Mix_LoadWAV("kill.wav");
    failure=Mix_LoadWAV("failure.wav");
    clearance=Mix_LoadWAV("clearence.wav");
    victory=Mix_LoadWAV("victory.wav");

    if(music==NULL||movement==NULL||kill==NULL||failure==NULL||clearance==NULL||victory==NULL)
    {
        return false;
    }

    return true;
}
bool Map::Show()
{
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF  )  );
    //apply_surface(0,0,background,screen);
    SDL_Rect rect;
    for(int i=0;i<M;i++)
    {
        for(int j=0;j<N;j++)
        {
            rect.x=i*W;
            rect.y=j*H;
            rect.w=W;
            rect.h=H;
            if(mat[i][j]==0)
            {
                //SDL_FillRect( screen, &rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF   )   );//用白色填涂
            }else
            {
                //SDL_FillRect( screen, &rect, SDL_MapRGB( screen->format, 0, 0, 0    )    );//用白色填涂
                //SDL_FillRect( screen, &rect, wall    );//用白色填涂
                apply_surface(rect.x,rect.y,wall,screen);
            }
        }
    }
    return true;
}
bool Hero::Show()
{
    if(x<0||y<0) return false;
    if(x>=mat.size()||y>=mat[x].size()) return false;

    SDL_Rect rect;
    rect.x=W*x;
    rect.y=H*y;
    rect.w=W;
    rect.h=H;
    //SDL_FillRect( screen, &rect, SDL_MapRGB( screen->format, 0x00, 0x00, 0xFF   )   );//用白色填涂
    apply_surface(rect.x,rect.y,hero,screen);
    return true;
}
bool Hero::MoveUp()//y--
{
    //can Hero move UP?
    if(y==0) return false;
    if(mat[x][y-1]==1)
    {
        return false;
    }
    if(mat[x][y-1]==2&&(y-1==0||mat[x][y-2]==1||mat[x][y-2]==2))
    {
        return false;
    }


    //下面是已经开始移动Hero了
    for(auto b:Bs)
    {
        int i=b->GetX();
        int j=b->GetY();

        if(i==x&&j==y-1)
        {
            b->MoveUp();
            break;
        }
    }

    if(mats[lvl][x][y]==1)
    {
        mat[x][y]=1;
    }else
    {
        mat[x][y]=0;
    }

    y--;
    return true;
}
bool Brain::MoveUp()
{
    if(x<0||y<0) return false;
    if(x>=mat.size()||y>=mat[x].size()) return false;

    if(mats[lvl][x][y]==3||mats[lvl][x][y]==0)
    {
        mat[x][y]=mats[lvl][x][y];
    }

    if(mats[lvl][x][y-1]==1||mat[x][y-1]==2)
    {
        return false;
    }

    mat[x][y-1]=2;
    y--;

    return true;
}
bool Hero::MoveDown()//y++
{
    if(y==mat[x].size()-1)
    {
        return false;
    }
    if(mat[x][y+1]==1)
    {
        return false;
    }

    if(mat[x][y+1]==2&&(y+2==mat[x].size()||mat[x][y+2]==2||mat[x][y+2]==1))
    {
        return false;
    }


    //Hero can move Down
    for(auto b:Bs)
    {
        int i=b->GetX();
        int j=b->GetY();

        if(i==x&&j==y+1)
        {
            b->MoveDown();
            break;
        }
    }
    if(mats[lvl][x][y]==1)
    {
        mat[x][y]=1;
    }else
    {
        mat[x][y]=0;
    }
    y++;
    return true;
}
bool Brain::MoveDown()
{
    if(x<0||y<0) return false;
    if(x>=mat.size()||y>=mat[x].size()) return false;

    if(mats[lvl][x][y]==3||mats[lvl][x][y]==0)
    {
        mat[x][y]=mats[lvl][x][y];
    }

    if(mats[lvl][x][y+1]==1||mat[x][y+1]==2)
    {
        return false;
    }

    mat[x][y+1]=2;
    y++;

    return true;
}
bool Hero::MoveLeft()//x--
{
    if(x==0) return false;
    if(mat[x-1][y]==1)
    {
        return false;
    }

    if(mat[x-1][y]==2&&(x-1==0||mat[x-2][y]==2||mat[x-2][y]==1))
    {
        return false;

    }


    //Hero can move Down
    for(auto b:Bs)
    {
        int i=b->GetX();
        int j=b->GetY();

        if(i==x-1&&j==y)
        {
            b->MoveLeft();
            break;
        }
    }

    if(mats[lvl][x][y]==1)
    {
        mat[x][y]=1;
    }else
    {
        mat[x][y]=0;
    }
    x--;
    return true;
}
bool Brain::MoveLeft()
{
    if(x<0||y<0) return false;
    if(x>=mat.size()||y>=mat[x].size()) return false;

    if(mats[lvl][x][y]==3||mats[lvl][x][y]==0)
    {
        mat[x][y]=mats[lvl][x][y];
    }

    if(mats[lvl][x-1][y]==1||mat[x-1][y]==2)
    {
        return false;
    }

    mat[x-1][y]=2;
    x--;

    return true;
}
bool Hero::MoveRight()//x++
{
    if(x==mat.size()-1) return false;
    if(mat[x+1][y]==1)
    {
        return false;
    }

        if(mat[x+1][y]==2&&(x+1==0||mat[x+2][y]==2||mat[x+2][y]==1))
        {
            return false;
        }


            //Hero can move Down
        for(auto b:Bs)
        {
            int i=b->GetX();
            int j=b->GetY();

            if(i==x+1&&j==y)
            {
                b->MoveRight();
                break;
            }
        }

    if(mats[lvl][x][y]==1)
    {
        mat[x][y]=1;
    x--;
    }else
    {
        mat[x][y]=0;
    }
    x++;
    return true;
}
bool Brain::MoveRight()
{
    if(x<0||y<0) return false;
    if(x>=mat.size()||y>=mat[x].size()) return false;

    if(mats[lvl][x][y]==3||mats[lvl][x][y]==0)
    {
        mat[x][y]=mats[lvl][x][y];

    }

    if(mats[lvl][x+1][y]==1||mat[x+1][y]==2)
    {
        return false;

    }

    mat[x+1][y]=2;
    x++;

    return true;

}

bool Disk::Show()
{
    if(x<0||y<0) return false;
    if(x>=mat.size()||y>=mat[x].size()) return false;

    SDL_Rect rect;
    rect.x=W*x;
    rect.y=H*y;
    rect.w=W;
    rect.h=H;
    //SDL_FillRect( screen, &rect, SDL_MapRGB( screen->format, 0x00, 0xFF, 0x00    )    );//用白色填涂
    apply_surface(rect.x,rect.y,disk,screen);
    return true;
}
bool Brain::Show()
{
    if(x<0||y<0) return false;
    if(x>=mat.size()||y>=mat[x].size()) return false;

    SDL_Rect rect;
    rect.x=W*x;
    rect.y=H*y;
    rect.w=W;
    rect.h=H;
    //SDL_FillRect( screen, &rect, SDL_MapRGB( screen->format, 0xFF, 0x00, 0x00    )    );//用白色填涂
    apply_surface(rect.x,rect.y,brain,screen);
    return true;
}
bool Monster::Show()
{
    if(x<0||y<0) return false;
    if(x>=mat.size()||y>=mat[x].size()) return false;

    SDL_Rect rect;
    rect.x=W*x;
    rect.y=H*y;
    rect.w=W;
    rect.h=H;
    //SDL_FillRect( screen, &rect, SDL_MapRGB( screen->format, 0x1F, 0xFF, 0xFF    )    );//用白色填涂
    apply_surface(rect.x,rect.y,monster,screen);
    return true;
}

void FreeAll()
{
    if(Ms!=NULL) free(Ms);
    Ms=NULL;
    for(auto b:Bs)
    {
        free(b);
    }
    Bs.resize(0,NULL);
    for(auto d:Ds)
    {
        free(d);
    }
    Ds.resize(0,NULL);
    if(Hr!=NULL) free(Hr);
    Hr=NULL;
    if(Mp!=NULL) free(Mp);
    Mp=NULL;
}
bool Monster::Move()
{
    if(x<0||y<0) return false;
    if(x>=mat.size()||y>=mat[x].size()) return false;
    int drt=-1;
    int i=100;
    while(i--)
    {
        drt=rand()%4;
        //drt=0;
        switch(drt)
        {
            case 0://y-- move up
                //if(y!=0&&mat[x][y-1]==0&&!(x==Hr->GetX()&&y-1==Hr->GetY()))
                if(y!=0&&mat[x][y-1]==0)
                {
                    //cout<<x<<":"<<y<<endl;
                    //cout<<mat<<endl;
                    y--;
                }else
                {
                    drt=-1;
                }
                break;

            case 1://x++
                //if(x!=mat.size()-1&&mat[x+1][y]==0&&!(x+1==Hr->GetX()&&y==Hr->GetY()))
                if(x!=mat.size()-1&&mat[x+1][y]==0)
                {
                    x++;
                }else
                {
                    drt=-1;
                }
                break;

            case 2://y++
                //if(y!=mat[x].size()-1&&mat[x][y+1]==0&&!(x==Hr->GetX()&&y+1==Hr->GetY()))
                if(y!=mat[x].size()-1&&mat[x][y+1]==0)
                {
                    y++;
                }else
                {
                    drt=-1;
                }
                break;

            case 3://x--
                //if(x!=0&&mat[x-1][y]==0&&!(x-1==Hr->GetX()&&y==Hr->GetY()))
                if(x!=0&&mat[x-1][y]==0)
                {
                    x--;
                }else
                {
                    drt=-1;
                }
                break;
        }
        if(drt!=-1)
        {
            return true;
        }
    }
    return false;
}

bool Win()
{
    for(auto d:Ds)
    {
        bool food=0;
        for(auto b:Bs)
        {
            if(d->GetX()==b->GetX()&&d->GetY()==b->GetY())
            {
                food=1;
                break;
            }
        }
        if(food==0) return false;
    }
    return true;
}

int main()
{
    bool quit=0;
    bool win=0;
    bool pause=0;
    bool lose=0;
    int mn_mv=0;

    if(init()==false)
    {
        return 1;
    }

    if(load_files()==false)
    {
        return 1;
    }


    if( Mix_PlayMusic( music, -1 ) == -1 )//ok
    {
        return 1;
    }


    Mp=new Map(0);


    if( SDL_Flip( screen ) == -1 )
    {
        return 1;
    }

    while(quit==0)
    {
        while(SDL_PollEvent(&event))
        {
            if(pause==0)
            {
                if(event.type==SDL_KEYDOWN)
                {
                    int Hpx=Hr->GetX();
                    int Hpy=Hr->GetY();
                    switch(event.key.keysym.sym)
                    {

                        case SDLK_PAUSE:
                            pause=1;
                            break;
                        case SDLK_UP:
                            Hr->MoveUp();
                            mn_mv++;
                            if(Mix_PlayChannel(-1,movement,0)==-1)
                            {
                                return 1;
                            }
                            break;

                        case SDLK_DOWN:
                            Hr->MoveDown();
                            mn_mv++;
                            if(Mix_PlayChannel(-1,movement,0)==-1)
                            {
                                return 1;
                            }
                            break;

                        case SDLK_LEFT:
                            Hr->MoveLeft();
                            mn_mv++;
                            if(Mix_PlayChannel(-1,movement,0)==-1)
                            {
                                return 1;
                            }
                            break;

                        case SDLK_RIGHT:
                            Hr->MoveRight();
                            if(Mix_PlayChannel(-1,movement,0)==-1)
                            {
                                return 1;
                            }
                            mn_mv++;
                            break;

                        case SDLK_r:
                            FreeAll();
                            Mp=new Map(lvl);
                            break;

                        case SDLK_s://save
                            {
                                ofstream sv("save",ios::in|ios::out|ios::binary);
                                if(!sv.is_open())
                                {
                                    cout<<"save not writed!";
                                    return 1;
                                }
                                for(int i=0;i<mat.size();i++)
                                {
                                    for(int j=0;j<mat[i].size();j++)
                                    {
                                        sv<<mat[i][j]<<' ';
                                    }
                                    sv<<"\r\n";
                                }
                                if(Ms)
                                    sv<<Hr->GetX()<<' '<<Hr->GetY()<<' ' <<Ms->GetX()<<' '<<Ms->GetY()<<' '<<lvl<<"\r\n";
                                else
                                    sv<<Hr->GetX()<<' '<<Hr->GetY()<<' ' <<-1<<' '<<-1<<' '<<lvl<<"\r\n";
                                sv.close();
                            }
                            break;
                        case SDLK_l://load
                            {
                                FreeAll();
                                ifstream infile("save",ios::in|ios::out|ios::binary);;
                                if(!infile.is_open())
                                {
                                    cout<<"file open fail"<<endl;
                                    exit(1);

                                }
                                string line,s;
                                int row=0;
                                while ( getline( infile, line  )  )
                                {
                                    if(row==M)
                                    {
                                        stringstream ss;//注意这个的头文件是 #include<sstream>
                                        ss<<line;
                                        int x,y;
                                        //while(ss)
                                        {
                                            ss>>x;
                                            ss>>y;
                                            Hr=new Hero(x,y);
                                            ss>>x;
                                            ss>>y;
                                            if(Ms==NULL) free(Ms);
                                            Ms=NULL;
                                            if(x!=-1&y!=-1)
                                            Ms=new Monster(x,y);
                                            ss>>lvl;
                                        }
                                        Mp=new Map();
                                        break;
                                    }
                                    int col=0;
                                    stringstream ss;//注意这个的头文件是 #include<sstream>
                                    ss<<line;
                                    while(ss)
                                    {
                                        int a;
                                        ss>>a;
                                        mat[row][col]=a;
                                        col++;
                                    }
                                    row++;
                                }
                                infile.close();
                            }
                            break;
                    }
                    if(mn_mv==2)
                    {
                        mn_mv=0;
                        if(Ms)
                        {
                            int Mpx=Ms->GetX();
                            int Mpy=Ms->GetY();
                            Ms->Move();
                            if(Ms->GetX()==Hpx&&Ms->GetY()==Hpy&&Hr->GetX()==Mpx&&Hr->GetY()==Mpy)
                            {
                                lose=1;
                            }
                        }
                    }
                }else if(event.type==SDL_QUIT)
                {
                    quit=1;
                    break;
                }
            }else if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_PAUSE)
            {
                pause=0;
            }
        }
        if(Mp)
            if( Mp->Show()==false) {return 1;}
        if(Ms)
            if(Ms->Show()==false) return 1;
        for(auto d:Ds)
        {
            if(d->Show()==false) return 1;
        }
        for(auto b:Bs)
        {
            if(b->Show()==false) return 1;
        }
        if(Hr)
            if( Hr->Show()==false) {return 1;}
        SDL_Delay(10);



        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }
        if(Ms!=NULL)
        {
            if(mn_mv==0&&Ms->GetX()==Hr->GetX()&&Ms->GetY()==Hr->GetY())//Be killed by Monster
            {
                if(Mix_PlayChannel(-1,failure,0)==-1)
                {
                    return 1;
                }
                lose=1;
            }else if(mn_mv!=0&&Ms->GetX()==Hr->GetX()&&Ms->GetY()==Hr->GetY())//kill Monster
            {
                if(Ms)
                free(Ms);
                Ms=NULL;
                if(Mix_PlayChannel(-1,kill,0)==-1)
                {
                    return 1;
                }
            }
        }
        if(Win())
        {
            lvl++;
            {
                FreeAll();
            }
            if(lvl<mats.size())
            {
                if(Mix_PlayChannel(-1,clearance,0)==-1)
                {
                    return 1;

                }
                Mp= new Map(lvl);
            }
            else
            {
                if(Mix_PlayChannel(-1,victory,0)==-1)
                {
                    return 1;
                }

                win=1;
                quit=1;
            }
        }
        if(lose)
        {
            lose=0;
            FreeAll();
            lvl=0;
            Mp=new Map(lvl);
            message = TTF_RenderText_Solid( font, "Lose Game,Restart in 2 seconds", textColor    );//ok
            apply_surface(0,0,failPic,screen);
            apply_surface(100,200,message,screen);
            char str[90];
            sprintf(str,"Push Box\tLose!!!!");
            SDL_WM_SetCaption(str, NULL     );

            if( SDL_Flip( screen ) == -1 )
            {
                return 1;
            }
            SDL_Delay(1000);

            message = TTF_RenderText_Solid( font, "Lose Game,Restart in 1 seconds", textColor     );//ok
            apply_surface(0,0,failPic,screen);
            apply_surface(100,200,message,screen);

            if( SDL_Flip( screen ) == -1 )
            {
                return 1;
            }
            SDL_Delay(1000);
        }
        while(quit==1&&win==1)
        {
            while(SDL_PollEvent(&event))
            {
                if(event.type==SDL_KEYDOWN)
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_r:
                            FreeAll();
                            lvl=0;
                            win=0;
                            quit=0;
                            Mp=new Map(lvl);
                            break;

                    }
                }else if(event.type==SDL_QUIT)
                {
                    quit=1;
                    win=0;
                    break;
                }
            }
            message = TTF_RenderText_Solid( font, "Win Game", textColor  );//ok
            apply_surface(0,0,winPic,screen);
            apply_surface(350,50,message,screen);
            message = TTF_RenderText_Solid( font, "press R to restart", textColor  );//ok
            apply_surface(280,550,message,screen);

            char str[90];
            sprintf(str,"Push Box\tWin!!!!!!!!!");
            SDL_WM_SetCaption(str, NULL     );
            if( SDL_Flip( screen ) == -1 )
            {
                return 1;
            }
            SDL_Delay(20);
        }
    }


    clean_up();


      return 0;
}
