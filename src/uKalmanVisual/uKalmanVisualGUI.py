#importa as bibliotecas

#GUI
from tkinter import *
from tkinter import ttk

#multithreading
import threading
import queue
from queue import Queue

#Process communication
import time
import zmq

#plotting
import matplotlib
matplotlib.use('TkAgg')
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2TkAgg
# implement the default mpl key bindings
from matplotlib.backend_bases import key_press_handler
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np





class GUI:
    #Declaracoes de objetos da classe:
    #Sao modificados as variaveis de todos os objetos instanciados
    def __init__(self,myqueue):
        #Declaracao de variaveis do objeto
        #Somente o objeto modificado altera
        self.guiqueue = myqueue
        #my arrays
        self.PK0 = []
        self.PK1 = []
        self.PK2 = []
        self.PK3 = []
        self.PK4 = []
        self.PK5 = []
        self.PK6 = []
        self.PK7 = []
        self.XERRO0 = []
        self.XERRO1 = []
        self.XERRO2 = []
        self.XERRO3 = []
        self.XERRO4 = []
        self.XERRO5 = []
        self.XERRO6 = []
        self.XERRO7 = []
        self.t = []
        
        #limite das escalas dos graficos
        #aceleracao
        self.Alim = 5
        #velocidade
        self.Vlim = 10
        #posicao
        self.Plim = 8
        #angulos
        self.PSIlim = 0.5
        self.WZlim = 1
        
        self.xUpperLim = 20
        self.xLowerLim = 0
        self.timestep = 10
        
        #start GUI
        self.startGUI()

    def startGUI(self):
        """Inicia a interface GUI"""
        #cria uma nova janela chamada root com titulo
        self.root = Tk()
        self.root.title("Kalman Graphics")

        #configura um frame na janela root
        mainframe = ttk.Frame(self.root, padding="0 0 0 0")
        mainframe.grid(column=0, row=0, sticky=(N, W, E, S))
        mainframe.columnconfigure(0, weight=0)
        mainframe.rowconfigure(0, weight=0)

        #Acrescentando um plot
        self.initPlot()
        self.canvas1 = FigureCanvasTkAgg(self.myfig1, master=mainframe)
        self.canvas1.get_tk_widget().grid(column=1,row=1)
        
        #define variaveis que estarao na janela
        self.receivedMessage = StringVar(self.root)

        #define um label
        messageLabel = ttk.Label(mainframe, textvariable=self.receivedMessage)
        messageLabel.grid(column=1, row=2, sticky=(W, E))

        #para cada uma das janelas ou filhos do mainframe eu coloco um padding  ao redor
        for child in mainframe.winfo_children():
            child.grid_configure(padx=0, pady=0)

        #schedule de uma funcao a cada 25ms
        self.root.after(10, self.processIncoming)

        #loop principal
        self.root.mainloop()

    
    def processIncoming(self):
        """Handle all messages currently in the queue, if any."""
        try:
            msg = self.guiqueue.get(False)
            (PK, XERRO, tt) = getDataFromString(msg)
            self.PK0.append(PK[0])
            self.PK1.append(PK[1])
            self.PK2.append(PK[2])
            self.PK3.append(PK[3])
            self.PK4.append(PK[4])
            self.PK5.append(PK[5])
            self.PK6.append(PK[6])
            self.PK7.append(PK[7])
            self.XERRO0.append(XERRO[0])
            self.XERRO1.append(XERRO[1])
            self.XERRO2.append(XERRO[2])
            self.XERRO3.append(XERRO[3])
            self.XERRO4.append(XERRO[4])
            self.XERRO5.append(XERRO[5])
            self.XERRO6.append(XERRO[6])
            self.XERRO7.append(XERRO[7])
            self.t.append(tt)
            
            
            #replota a cada dado novo
            self.updatePlot()
            
        except queue.Empty:
            #print("Fila Vazia")
            pass

        self.root.after(25, self.processIncoming)

    def initPlot(self):
        self.myfig1 = Figure(figsize=(16,9), dpi=60)
        self.VXplot = self.myfig1.add_subplot(241)
        self.VYplot = self.myfig1.add_subplot(242)
        self.PXplot = self.myfig1.add_subplot(243)
        self.PYplot = self.myfig1.add_subplot(244)
        self.PSIplot = self.myfig1.add_subplot(245)
        self.AXplot = self.myfig1.add_subplot(246)
        self.AYplot = self.myfig1.add_subplot(247)
        self.WZplot = self.myfig1.add_subplot(248)    
        
        #incializando os plots
        #VX - index 0
        self.VXplot.set_title("Vx Error")
        self.VXplot.axis([self.xLowerLim,self.xUpperLim,-self.Vlim,self.Vlim])
        self.VXplot.grid(True)
        self.VXsupP, = self.VXplot.plot(self.t, 3*self.PK0,'r')
        self.VXlowP, = self.VXplot.plot(self.t,-3*self.PK0,'r')
        self.VXXERRO, = self.VXplot.plot(self.t,self.XERRO0,'k')
        
        #VY - index 1
        self.VYplot.set_title("Vy Error")
        self.VYplot.axis([self.xLowerLim,self.xUpperLim,-self.Vlim,self.Vlim])
        self.VYplot.grid(True)
        self.VYsupP, = self.VYplot.plot(self.t, 3*self.PK1,'r')
        self.VYlowP, = self.VYplot.plot(self.t,-3*self.PK1,'r')
        self.VYXERRO, = self.VYplot.plot(self.t,self.XERRO1,'k')
        
        #PX - index 2
        self.PXplot.set_title("Px Error")
        self.PXplot.axis([self.xLowerLim,self.xUpperLim,-self.Plim,self.Plim])
        self.PXplot.grid(True)
        self.PXsupP, = self.PXplot.plot(self.t, 3*self.PK2,'r')
        self.PXlowP, = self.PXplot.plot(self.t,-3*self.PK2,'r')
        self.PXXERRO, = self.PXplot.plot(self.t,self.XERRO2,'k')
        
        #PY - index 3
        self.PYplot.set_title("Py Error")
        self.PYplot.axis([self.xLowerLim,self.xUpperLim,-self.Plim,self.Plim])
        self.PYplot.grid(True)
        self.PYsupP, = self.PYplot.plot(self.t, 3*self.PK3,'r')
        self.PYlowP, = self.PYplot.plot(self.t,-3*self.PK3,'r')
        self.PYXERRO, = self.PYplot.plot(self.t,self.XERRO3,'k')
        
        #PSI - index 4
        self.PSIplot.set_title("Psi Error")
        self.PSIplot.axis([self.xLowerLim,self.xUpperLim,-self.PSIlim,self.PSIlim])
        self.PSIplot.grid(True)
        self.PSIsupP, = self.PSIplot.plot(self.t, 3*self.PK4,'r')
        self.PSIlowP, = self.PSIplot.plot(self.t,-3*self.PK4,'r')
        self.PSIXERRO, = self.PSIplot.plot(self.t,self.XERRO4,'k')
        
        #AX - index 5
        self.AXplot.set_title("Ax Bias")
        self.AXplot.axis([self.xLowerLim,self.xUpperLim,-self.Alim,self.Alim])
        self.AXplot.grid(True)
        self.AXsupP, = self.AXplot.plot(self.t, 3*self.PK5,'r')
        self.AXlowP, = self.AXplot.plot(self.t,-3*self.PK5,'r')
        self.AXXERRO, = self.AXplot.plot(self.t,self.XERRO5,'k')
        
        #AY - index 6
        self.AYplot.set_title("Ay Bias")
        self.AYplot.axis([self.xLowerLim,self.xUpperLim,-self.Alim,self.Alim])
        self.AYplot.grid(True)
        self.AYsupP, = self.AYplot.plot(self.t, 3*self.PK6,'r')
        self.AYlowP, = self.AYplot.plot(self.t,-3*self.PK6,'r')
        self.AYXERRO, = self.AYplot.plot(self.t,self.XERRO6,'k')
        
        #WZ - index 7
        self.WZplot.set_title("Wz Bias")
        self.WZplot.axis([self.xLowerLim,self.xUpperLim,-self.WZlim,self.WZlim])
        self.WZplot.grid(True)
        self.WZsupP, = self.WZplot.plot(self.t, 3*self.PK7,'r')
        self.WZlowP, = self.WZplot.plot(self.t,-3*self.PK7,'r')
        self.WZXERRO, = self.WZplot.plot(self.t,self.XERRO7,'k')


    
    
    def updatePlot(self):
    #Update nos plots
        #Reorganizando os intervalos de tempo
        if self.t[-1] > self.xUpperLim - self.timestep/3:
            self.xLowerLim = self.xLowerLim + self.timestep
            self.xUpperLim = self.xUpperLim + self.timestep
            self.VXplot.axis([self.xLowerLim,self.xUpperLim,-self.Vlim,self.Vlim])
            self.VYplot.axis([self.xLowerLim,self.xUpperLim,-self.Vlim,self.Vlim])
            self.PXplot.axis([self.xLowerLim,self.xUpperLim,-self.Plim,self.Plim])
            self.PYplot.axis([self.xLowerLim,self.xUpperLim,-self.Plim,self.Plim])
            self.PSIplot.axis([self.xLowerLim,self.xUpperLim,-self.PSIlim,self.PSIlim])
            self.AXplot.axis([self.xLowerLim,self.xUpperLim,-self.Alim,self.Alim])
            self.AYplot.axis([self.xLowerLim,self.xUpperLim,-self.Alim,self.Alim])
            self.WZplot.axis([self.xLowerLim,self.xUpperLim,-self.WZlim,self.WZlim])
           
        #Acrescentando os novos dados 
        #VX
        npPK0 = np.array(self.PK0)
        self.VXsupP.set_ydata(3*npPK0)
        self.VXsupP.set_xdata(self.t)
        self.VXlowP.set_ydata(-3*npPK0)
        self.VXlowP.set_xdata(self.t)
        self.VXXERRO.set_ydata(self.XERRO0)
        self.VXXERRO.set_xdata(self.t)
        
        #VY
        npPK1 = np.array(self.PK1)
        self.VYsupP.set_ydata(3*npPK1)
        self.VYsupP.set_xdata(self.t)
        self.VYlowP.set_ydata(-3*npPK1)
        self.VYlowP.set_xdata(self.t)
        self.VYXERRO.set_ydata(self.XERRO1)
        self.VYXERRO.set_xdata(self.t)
        
        #PX
        npPK2 = np.array(self.PK2)
        self.PXsupP.set_ydata(3*npPK2)
        self.PXsupP.set_xdata(self.t)
        self.PXlowP.set_ydata(-3*npPK2)
        self.PXlowP.set_xdata(self.t)
        self.PXXERRO.set_ydata(self.XERRO2)
        self.PXXERRO.set_xdata(self.t)
        
        #PY
        npPK3 = np.array(self.PK3)
        self.PYsupP.set_ydata(3*npPK3)
        self.PYsupP.set_xdata(self.t)
        self.PYlowP.set_ydata(-3*npPK3)
        self.PYlowP.set_xdata(self.t)
        self.PYXERRO.set_ydata(self.XERRO3)
        self.PYXERRO.set_xdata(self.t)
        
        #PSI
        npPK4 = np.array(self.PK4)
        self.PSIsupP.set_ydata(3*npPK4)
        self.PSIsupP.set_xdata(self.t)
        self.PSIlowP.set_ydata(-3*npPK4)
        self.PSIlowP.set_xdata(self.t)
        self.PSIXERRO.set_ydata(self.XERRO4)
        self.PSIXERRO.set_xdata(self.t)
        
        #AX
        npPK5 = np.array(self.PK5)
        self.AXsupP.set_ydata(3*npPK5)
        self.AXsupP.set_xdata(self.t)
        self.AXlowP.set_ydata(-3*npPK5)
        self.AXlowP.set_xdata(self.t)
        self.AXXERRO.set_ydata(self.XERRO5)
        self.AXXERRO.set_xdata(self.t)
        
        #AY
        npPK6 = np.array(self.PK6)
        self.AYsupP.set_ydata(3*npPK6)
        self.AYsupP.set_xdata(self.t)
        self.AYlowP.set_ydata(-3*npPK6)
        self.AYlowP.set_xdata(self.t)
        self.AYXERRO.set_ydata(self.XERRO6)
        self.AYXERRO.set_xdata(self.t)
        
        #WZ
        npPK7 = np.array(self.PK7)
        self.WZsupP.set_ydata(3*npPK7)
        self.WZsupP.set_xdata(self.t)
        self.WZlowP.set_ydata(-3*npPK7)
        self.WZlowP.set_xdata(self.t)
        self.WZXERRO.set_ydata(self.XERRO7)
        self.WZXERRO.set_xdata(self.t)
        
        
        self.canvas1.draw()



class Comms:
    def __init__(self,myqueue):
        #Declaracao de variaveis do objeto
        #Somente o objeto modificado altera
        self.commsqueue = myqueue

    def receiveData(self):
        """Inicia a comunicacao com ZeroMQ"""
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.PAIR)
        #self.socket.connect("tcp://localhost:5556")
        self.socket.connect("ipc:///tmp/mysocket")
        print("Communication via IPC - Mac and Linux Only")
        #Envia uma mensagem pedindo para comecar
        startstr = "START"
        self.socket.send(startstr.encode('utf-8'))
        time.sleep(1)
        #Recebe os dados
        while True:
            contents = self.socket.recv()
            self.commsqueue.put(contents)




def getDataFromString(contents):
    "Retirando os valores recebidos"
    try:
        contentsDecoded = contents.decode('utf-8')
        contentsList = contentsDecoded.split(' ')
        PK = [float(contentsList[0]), float(contentsList[1]), float(contentsList[2]), float(contentsList[3]), float(contentsList[4]), float(contentsList[5]), float(contentsList[6]), float(contentsList[7])]
        XERRO = [float(contentsList[8]), float(contentsList[9]), float(contentsList[10]), float(contentsList[11]), float(contentsList[12]), float(contentsList[13]), float(contentsList[14]), float(contentsList[15])]
        t = float(contentsList[16])
        return (PK, XERRO,t)
    except:
        print("Erro na decodificacao")
        pass


def main():
    """main method"""
    appQueue = Queue()
    myComms = Comms(appQueue)
    #comeca a receber os dados em uma outra thread 
    #(o zmq ja recebe em outra thread, mas essa e para atualizar os dados da interface)
    commsThread = threading.Thread(target = myComms.receiveData)
    commsThread.daemon = True
    commsThread.start()
    myGUI = GUI(appQueue)


if __name__ == "__main__":
    main()