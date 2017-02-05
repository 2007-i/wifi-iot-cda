//��� ������ �������� 9 �������� ,���������� 0 
//��� ������  � ������ ������� � ���������,�������� �� ����������� �����������,��������� ����� ����� ������ ����� �������.������� ������� ����������� ���������,������ �����.����� ������ ����- ��������� ����,��������� ���,,����������.���������.��������� �� ������� ������

int32_t tempSave=1;
int32_t tempVhod;
uint8_t vremia_oshibki=0;
bool time=0;
uint8_t gpio=16; //����� ���� �����
void ICACHE_FLASH_ATTR startfunc() {
}

void ICACHE_FLASH_ATTR timerfunc(uint32_t timersrc) { 

    tempVhod = data1wire[0] ;//������ ����������� dsw1
	
    if ( sensors_param.cfgdes[2] < 0 || 23 < sensors_param.cfgdes[2] ) 
		sensors_param.cfgdes[2] = 0 ; //���� 1
	
    if ( sensors_param.cfgdes[3] < 0 || 59 < sensors_param.cfgdes[3] ) 
		sensors_param.cfgdes[3] = 0 ; //������ 1
	
    if ( sensors_param.cfgdes[6] < 0 || 23 < sensors_param.cfgdes[6] ) 
		sensors_param.cfgdes[6] = 0 ; //���� 2
	
    if ( sensors_param.cfgdes[7] < 0 || 59 < sensors_param.cfgdes[7] ) 
		sensors_param.cfgdes[7] = 0 ; //������ 2
	
    if ( sensors_param.cfgdes[8] < 0 || 3 < sensors_param.cfgdes[8] ) 
		sensors_param.cfgdes[8] = 0 ; //�����
	
    if ( tempVhod != 0 && tempVhod != 850 && tempVhod != 2550 ) { 
        tempSave = tempVhod ; 
    } else { 
        vremia_oshibki++ ; 
    }
	
    if ( 60 <= vremia_oshibki ){ //���� ������ �������� �������� ������ �� ������������ ��������,���� �� �� ������������� ,����� ��� ������ � �����y
        tempSave = tempVhod ; 
        vremia_oshibki = 0 ; 
    }
	
    if ( sensors_param.cfgdes[2] < sensors_param.cfgdes[6] ) {
        if ( ( time_loc.hour == sensors_param.cfgdes[2] && sensors_param.cfgdes[3] <= time_loc.min ) || ( sensors_param.cfgdes[2] < time_loc.hour && time_loc.hour <sensors_param.cfgdes[6] ) || ( time_loc.hour == sensors_param.cfgdes[6 ] && time_loc.min < sensors_param.cfgdes[7] ) ) {
            time = 1 ; 
        } else { 
            time =0 ; 
        }
    } else {
        if ( ( time_loc.hour == sensors_param.cfgdes[2] && sensors_param.cfgdes[3] <= time_loc.min ) || ( sensors_param.cfgdes[2] < time_loc.hour && time_loc.hour <24 ) || ( 0 <= time_loc.hour && time_loc.hour < sensors_param.cfgdes[6] ) || ( time_loc.hour == sensors_param.cfgdes[6] && time_loc.min < sensors_param.cfgdes[7] ) ) { 
            time = 1 ; 
        } else { 
            time = 0 ; 
        }
    }
	
    if ( sensors_param.cfgdes[8] == 0 ) 
		digitalWrite(gpio,0);//���� ���������
	
    if ( sensors_param.cfgdes[8] == 1 ){ //�����
        if ( time == 1 ) {
            if ( sensors_param.cfgdes[1] < tempSave ) 
				digitalWrite(gpio,1);
            if ( tempSave < sensors_param.cfgdes[0] ) 
				digitalWrite(gpio,0);
        } else { 
            if ( sensors_param.cfgdes[5] < tempSave ) 
				digitalWrite(gpio,1);
            if ( tempSave < sensors_param.cfgdes[4] ) 
				digitalWrite(gpio,0);
        } 
    }
	
    if ( sensors_param.cfgdes[8] == 2 ){ //���������
        if ( time == 1 ) {
            if ( sensors_param.cfgdes[1] < tempSave ) 
				digitalWrite(gpio,0);
            if ( tempSave < sensors_param.cfgdes[0] ) 
				digitalWrite(gpio,1);
        } else { 
            if ( sensors_param.cfgdes[5] < tempSave ) 
				digitalWrite(gpio,0);
            if ( tempSave < sensors_param.cfgdes[4] ) 
				digitalWrite(gpio,1);
        } 
    } 
	
    if ( sensors_param.cfgdes[8] == 3 ) digitalWrite(gpio,1);//��� ��������� 
    }
	
void webfunc(char *pbuf) { //��� ���������� �� �����
    if ( GPIO_ALL_GET(gpio)!=0 && sensors_param.cfgdes[8] ==1 ) {
        os_sprintf(HTTPBUFF,"</div><br><div class='h' style='background: #73c140 '> ��������� : <font color='blue'>�������</font> . &nbsp;&nbsp;");
    }
	
    if ( GPIO_ALL_GET(gpio)!=0 && sensors_param.cfgdes[8] ==2 ) {
        os_sprintf(HTTPBUFF,"</div><br><div class='h' style='background: #73c140 '> ��������� : <font color='yellow'>�������</font> . &nbsp;&nbsp;");
    } 
	
    if ( GPIO_ALL_GET(gpio)!=0 && sensors_param.cfgdes[8] ==3 ) {
        os_sprintf(HTTPBUFF,"</div><br><div class='h' style='background: #73c140 '> ��������� : <font color='red'>�������</font> . &nbsp;&nbsp;");
    } 
	
    if ( GPIO_ALL_GET(gpio)==0 ) {
        os_sprintf(HTTPBUFF,"</div><br><div class='h' style='background: #73c140 '> ��������� : <font color='black'>��������</font> . &nbsp;");
    }
	
    if(sensors_param.cfgdes[8] ==0){
		os_sprintf(HTTPBUFF," ����� :<b><font color='black'> �������� ���������</font></b>");
	}
	
    if(sensors_param.cfgdes[8] ==1){
		os_sprintf(HTTPBUFF," ����� :<b><font color='blue'> ����������</font></b>");
	}
	
    if(sensors_param.cfgdes[8] ==2){
		os_sprintf(HTTPBUFF," ����� :<b><font color='yellow'> �����������</font></b>");
	}
	
    if(sensors_param.cfgdes[8] ==3){
		os_sprintf(HTTPBUFF," ����� :<b><font color='red'> ������� ���������</font></b>");
	}
	
    if(sensors_param.cfgdes[8] ==1){
		os_sprintf(HTTPBUFF,"</div><div class='c'><div class='main'><form action=configdes><pre><table border='0'class='catalogue'><tr style='background-color: yellow'><td> ���� </td><td> ������ </td><td> ���� ����</td><td> ���� ��� </td></tr><tr><td>c <INPUT size=2 NAME='cfg3'value='%02d'></td><td> <INPUT size=2 NAME='cfg4'value='%02d'></td><td> ���� <INPUT size=2 NAME='cfg1' value='%s'> �C</td>",sensors_param.cfgdes[2],sensors_param.cfgdes[3], fltostr(sensors_param.cfgdes[0]));
	}
	else{
		os_sprintf(HTTPBUFF,"</div><div class='c'><div class='main'><form action=configdes><pre><table border='0'class='catalogue'><tr style='background-color: yellow'><td> ���� </td><td> ������ </td><td> ���� ��� </td><td> ���� ����</td></tr><tr><td>c <INPUT size=2 NAME='cfg3'value='%02d'></td><td> <INPUT size=2 NAME='cfg4'value='%02d'></td><td> ���� <INPUT size=2 NAME='cfg1' value='%s'> �C</td>",sensors_param.cfgdes[2],sensors_param.cfgdes[3], fltostr(sensors_param.cfgdes[0]));
	}
    os_sprintf(HTTPBUFF+os_strlen(HTTPBUFF),"<td> ���� <INPUT size=2 NAME='cfg2'value='%s'> �C</td></tr>",fltostr(sensors_param.cfgdes[1]));
    os_sprintf(HTTPBUFF+os_strlen(HTTPBUFF),"<tr><td>c <INPUT size=2 NAME='cfg7'value='%02d'></td><td> <INPUT size=2 NAME='cfg8'value='%02d'></td><td> ���� <INPUT size=2 NAME='cfg5' value='%s'> �C</td>",sensors_param.cfgdes[6],sensors_param.cfgdes[7], fltostr(sensors_param.cfgdes[4]));
    os_sprintf(HTTPBUFF+os_strlen(HTTPBUFF),"<td> ���� <INPUT size=2 NAME='cfg6'value='%s'> �C</td></tr></table><br><input type='hidden'name='cfg9' value='%d'><input type='hidden'name='st'value=3><input type=submit value='�������'onclick='pb(cfg1);pb(cfg2);pb(cfg5);pb(cfg6)'> <input type=submit value='�����'onclick='pb(cfg1);pb(cfg2);pb(cfg5);pb(cfg6);eb(cfg9)'></pre></form></div><script>function pb(x){x.value = x.value*10};function eb(x){x.value =Number(x.value)+ 1}</script>",fltostr(sensors_param.cfgdes[5]),sensors_param.cfgdes[8]);
    
}
