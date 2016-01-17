%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Script para plotar os dados de Navegacao retirados do .alog
%   Grafico da trajetoria perda sinal
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear;clc;close all;

%Abrindo o arquivo alog e lendo as variaveis
%fid = fopen('nav_raio5.alog'); raio =5;
fid = fopen('filterdata.alog'); raio =5;
data = textscan(fid, '%f%s%s%f','CommentStyle','%');
fclose(fid);

POSX.time(1)=0; POSX.value(1)=0;
POSY.time(1)=0; POSY.value(1)=0;
PSI.time(1)=0; PSI.value(1)=0;
ACCELX.time(1)=0; ACCELX.value(1)=0;
ACCELY.time(1)=0; ACCELY.value(1)=0;
ACCELZ.time(1)=0; ACCELZ.value(1)=0;
GYROX.time(1)=0; GYROX.value(1)=0;
GYROY.time(1)=0; GYROY.value(1)=0;
GYROZ.time(1)=0; GYROZ.value(1)=0;



for i=1:length(data{1,1})
    if strcmp(data{1,2}(i),'NAV_X')
        j = length(POSX.value) +1;
        POSX.time(j)=data{1,1}(i);
        POSX.value(j)=data{1,4}(i);
    elseif strcmp(data{1,2}(i),'NAV_Y')
        j = length(POSY.value) +1;
        POSY.time(j)=data{1,1}(i);
        POSY.value(j)=data{1,4}(i);
    elseif strcmp(data{1,2}(i),'NAV_HEADING')
        j = length(PSI.value) +1;
        PSI.time(j)=data{1,1}(i);
        PSI.value(j)=data{1,4}(i);
    elseif strcmp(data{1,2}(i),'NAV_GYROX')
        j = length(GYROX.value) +1;
        GYROX.time(j)=data{1,1}(i);
        GYROX.value(j)=data{1,4}(i);
    elseif strcmp(data{1,2}(i),'NAV_GYROY')
        j = length(GYROX.value) +1;
        GYROY.time(j)=data{1,1}(i);
        GYROY.value(j)=data{1,4}(i);
    elseif strcmp(data{1,2}(i),'NAV_GYROZ')
        j = length(GYROZ.value) +1;
        GYROZ.time(j)=data{1,1}(i);
        GYROZ.value(j)=data{1,4}(i);
    elseif strcmp(data{1,2}(i),'NAV_ACCELX')
        j = length(GYROX.value) +1;
        ACCELX.time(j)=data{1,1}(i);
        ACCELX.value(j)=data{1,4}(i);
    elseif strcmp(data{1,2}(i),'NAV_ACCELY')
        j = length(GYROX.value) +1;
        ACCELY.time(j)=data{1,1}(i);
        ACCELY.value(j)=data{1,4}(i);
    elseif strcmp(data{1,2}(i),'NAV_ACCELZ')
        j = length(GYROX.value) +1;
        ACCELZ.time(j)=data{1,1}(i);
        ACCELZ.value(j)=data{1,4}(i);
    end
end


figure
hold on
    plot(POSX.time,POSX.value,'b','LineWidth',2)
    grid on    
    title('Medida do GPS em X');
    xlabel('time [s]')
    ylabel('[m]')
 hold off

figure
hold on
    plot(POSY.time,POSY.value,'b','LineWidth',2)
    grid on    
    title('Medida do GPS em Y');
    xlabel('time [s]')
    ylabel('[m]')
 hold off
 
figure
hold on
    plot(PSI.time,PSI.value,'b','LineWidth',2)
    grid on    
    title('Medida da bussola');
    xlabel('time [s]')
    ylabel('[degrees]')
 hold off

figure
hold on
    plot(ACCELX.time,ACCELX.value,'b','LineWidth',2)
    grid on    
    title('Acelerometro em X');
    xlabel('time [s]')
    ylabel('')
 hold off
 
 figure
hold on
    plot(ACCELY.time,ACCELY.value,'b','LineWidth',2)
    grid on    
    title('Acelerometro em Y');
    xlabel('time [s]')
    ylabel('')
 hold off
 
 figure
hold on
    plot(ACCELZ.time,ACCELZ.value,'b','LineWidth',2)
    grid on    
    title('Acelerometro em Z');
    xlabel('time [s]')
    ylabel('')
 hold off
 
 