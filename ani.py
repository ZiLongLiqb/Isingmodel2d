import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import imageio
from ffmpy import FFmpeg
from PIL import Image
import math
from scipy import interpolate

'''outfile="my.gif";
fig=plt.figure();
ims=[]
with open("result_xx.txt","r") as f:
	result=f.read();
	a=result.split();
	a=np.array(a,dtype="int");

for i in range(1000):
	im=plt.imshow(a[i*3600:(i+1)*3600].reshape((60,60)),cmap="binary",animated=True);
	ims.append([im]);

anim=animation.ArtistAnimation(fig,ims,interval=20,blit=True,repeat_delay=1000)
anim.save("movie.gif",writer="pillow");
plt.show()'''

#plt.imshow(a[0:3600].reshape((60,60)),cmap="binary");
#plt.show();

'''image_list=[]
for i in range(100):
	image_list.append("%d.jpg"%i)

frames=[]
for image_name in image_list:
	frames.append(imageio.imread(image_name))

imageio.mimsave(outfile,frames,"GIF",duration=0.35)'''

with open("result_m.txt") as f:
	result=f.read();
	a=result.split("\n");


'''for i in range(len(a)):
	a[i]=a[i]/(i+1)/3600;


plt.figure();
plt.scatter(range(len(a)),a,s=2);
plt.plot(range(len(a)),a);
plt.show();'''

m_ave=[]
m_squ=[]
b=[];
length=[]
for i in range(200):
	c=a[i].split(" ");
	d=[]
	for j in range(len(c)-1):
		d.append(int(c[j]));
	length.append(len(d));
	b.append(d);

'''for i in range(len(b)):
	ave=0
	squ=0
	for j in range(len(b[i])):
		ave+=b[i][j];
		squ+=pow(b[i][j],2)
	ave=ave/len(b[i]);
	squ=squ/len(b[i]);
	m_ave.append(pow(ave,2));
	m_squ.append(squ);
result=[]
T=[]
for i in range(50):
	result.append(-(m_ave[i]-m_squ[i])/pow(2.2+0.008*i,2));
	T.append(2.2+0.008*i)

plt.figure()
plt.scatter(T,result,s=2);
#plt.plot(T,result);
plt.show();'''

for i in range(len(b)):
	ave=0
	squ=0
	for j in range(len(b[i])):
		ave+=b[i][j];
		squ+=pow(b[i][j],2)
	ave=ave/len(b[i]);
	squ=squ/len(b[i]);
	m_ave.append(ave);
	m_squ.append(squ);

t=[]
for i in range(len(b)):
	corre=0
	m_ave_before=0
	m_ave_after=0
	for j in range(len(b[i])-10):
		corre+=b[i][j]*b[i][j+10];
		m_ave_before+=b[i][j];
		m_ave_after+=b[i][j+10];
	corre=corre/(len(b[i])-10);
	m_ave_before=m_ave_before/(len(b[i])-10);
	m_ave_after=m_ave_after/(len(b[i])-10);
	t.append(10/(-np.log(np.abs(corre-m_ave_before*m_ave_after)/(m_squ[i]-pow(m_ave[i],2)))))

error=[]
for i in range(len(t)):
	error.append(np.sqrt((1+2*t[i])/(length[i])*(m_squ[i]-pow(m_ave[i],2))))
T=[]
for i in range(200):
	T.append(1.5+0.01*i)

m_ave=[m_ave[i]/3600 for i in range(len(m_ave))];
error=[error[i]/3600 for i in range(len(m_ave))];
plt.figure();
plt.tick_params(axis="both", which="both", bottom="off", top="off",
                labelbottom="on", left="on", right="off", labelleft="on")
plt.errorbar(T,m_ave,yerr=error,fmt="",color="b",capsize=4,ecolor="r");
plt.legend(['average_energy'],
           loc='upper right',
           numpoints=1,
           fancybox=False)
plt.scatter(T,m_ave,s=20);   
plt.xlabel("$T/K$")
plt.ylabel("$H$")
plt.title("Condition $\mathcal{J}=-1,\mathcal{h}=0$")
plt.show();
'''corre=0
m_ave_before=0
m_ave_after=0
for i in range(len(b[0])-10):
	corre+=b[0][i]*b[0][i+10];
	m_ave_before+=b[0][i];
	m_ave_after+=b[0][i+10];

corre=corre/(len(b[0])-10);
m_ave_before=m_ave_before/(len(b[0])-10);
m_ave_after=m_ave_after/(len(b[0])-10);
print(corre);
print(m_ave_before*m_ave_after);
print(m_squ[0]-pow(m_ave[0],2));
print(10/(-np.log((corre-m_ave_before*m_ave_after)/(m_squ[0]-pow(m_ave[0],2)))));'''

