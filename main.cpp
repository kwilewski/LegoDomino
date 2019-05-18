#include <hFramework.h>
#include <DistanceSensor.h>

using namespace hFramework;

//silniki spis treści:
//1 naped
//2 wypychacz
//3 brama
//4 skrecanie

bool prior=false;
int skret=0;   
bool startst=false,turbo=false,podwojne=false, wsteczny=false; 
//int n=0;
//int wunszskret=0;
//int ws=0;
//bool wszs=false;
bool wezyk=false;


void skretlewy(){
	if(skret!=(-1)){              
		hMot4.setPower(-170);
		sys.delay(1000);
		hMot4.setPower(0);
		skret--;		
	}
}

void skretprawy(){
	if(skret!=(1)){                   
		hMot4.setPower(170);
		sys.delay(1000);
		hMot4.setPower(0);
		skret++;
	}
}

void startstop(){
    if(startst){
        startst=false;
    }
    else{
        startst=true;
    }
}

void turbojazda(){
    turbo=!turbo;
    if(wsteczny){
        wsteczny=false;
    } 
}

void podwojneDomino(){
    if(podwojne){
        podwojne=false;
   }
    else{
        podwojne=true;
    }
}

void lecimytutaj(){
    while(skret!=(1)){                 
		hMot4.setPower(200);
		sys.delay(1000);
		hMot4.setPower(0);
		skret++;
	}
}

void wonsz(){
    wezyk=!wezyk;
}



void buttons_thread_loop()
{
    for(;;){
        if(!prior){
            char d;																										//	VERYFCKNGIMPORTANT
            if(hExt1.serial.available()>0){																				//		!!!!!!
                d=hExt1.serial.getch();
                printf("d=%c",d);
            }

            switch(d){
                case 'a':
                skretlewy();
                break;
                
                case 'd':
                skretprawy();
                break;
                
                case 's':
                startstop();
                break;

                case 'w':
                turbojazda();
                break;

                case 'q':
                podwojneDomino();
                break;

                case 'e':
                lecimytutaj();
                break;

                case 'r':
                wonsz();
                break;

                case 'x':
                wsteczny=!wsteczny;
                if(turbo){
                    turbo=false;
                }
                break;
            };
            d='z'; 	
        }
    }

}


void hMain()
{
	sys.taskCreate(buttons_thread_loop);

	sys.setLogDev(&Serial);
	hExt1.serial.init(9600,Parity::Odd,StopBits::One);

	hModules::DistanceSensor sens(hSens1.getBaseSens());
	int POWER;

    for(;;){
	
        if (startst){ 
            POWER=290;
            int dist=sens.getDistance();
            printf("dist %d\r\n", dist); 
            //sekwencja wypychania dominka jesli dystans jest powyzej 30cm
            if(dist>30 || dist==-1){
                if(turbo){
                    hMot1.setPower(1000);
                    sys.delay(100);
                }else if(wsteczny){
                    hMot1.setPower(-600);
                    sys.delay(100);
                }else{
                    hMot1.setPower(0);
                    //wypychanie domina
                    hMot2.setPower(350); 
                    sys.delay(880);
                    //powrot wypychaczki
                    hMot2.setPower(-350);
                    sys.delay(930);
                    hMot2.setPower(0);
                    if(podwojne){
                        //wypychanie domina
                        hMot2.setPower(350); 
                        sys.delay(880);
                        //powrot wypychaczki
                        hMot2.setPower(-350);
                        sys.delay(930);
                    }
                    sys.delay(300);
                    //otwieranie bramy
                    hMot3.setPower(-300);  
                    sys.delay(375);
                    hMot3.setPower(0);
                    sys.delay(400);
                    //sprawdzenie czy kola sa skrecone
                    if(skret==0){
                        if(wezyk){
                            POWER=430;
                        }
                        else{
                            POWER=330;    //z 290
                        } 
                    }
                    else{
                        if(wezyk){
                            POWER=490;
                        }
                        else{
                            POWER=360;    //z 350
                        } 
                    }
                    hMot1.setPower(POWER); 
                    sys.delay(800);
                    hMot1.setPower(0);
                    //zamkniecie bramy 
                    hMot3.setPower(220); 
                    sys.delay(400);
                    hMot3.setPower(0);
                    }
            }
            else{
                startst=false;
                hMot1.setPower(0);
                if(wsteczny){
                    hMot1.setPower(-600);
                    sys.delay(100);
                }
            }
        }else{
            hMot1.setPower(0);
            sys.delay(100);
        }
    }
}