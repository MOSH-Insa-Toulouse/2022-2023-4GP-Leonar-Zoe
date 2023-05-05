# SOMMAIRE :

  ## Description du Projet
  ## Livrable
  ## Arduino et son programme
  ## KiCad
  ## Fabrication de la board
  ## Banc de Test et Resultat

# 1 - Description du Projet : 
  
  Ce projet fait suite aux travaux de chercheurs dans le domaine des capteurs afin d’évaluer les potentialités de leur technologie low-tech. Il balayera l’ensemble du domaine du capteur jusqu’à la réalisation d’une datasheet et du banc de test associé au capteur.  Vous réaliserez ainsi des capteurs low-tech à base de graphite, en ferez une analyse critique argumentée pour en cerner les potentialités et le cas échéant proposer des solutions d’amélioration.
  
  ![Capteur](https://user-images.githubusercontent.com/123975808/236404362-1486d5c3-1306-4a53-af7e-c833399e0547.jpeg)

  
# 2 - Livrable : 
  
  Sont attendus à la fin de ce projet, une carte électronique montée sur une carte arduino UNO qui nous permettra de récupérer les valeurs de résistance d'un capteur de graphite réalisé sur une feuille de papier (140g/m²), le programme Arduino pour piloter la carte, une application Android qui affichera les valeurs de résistance en temps réel, un banc de test et la datasheet du capteur de graphite low-tech.
  
  Il a eu un changement dans le montage en cours de projet, en effet, nous étions partis pour utiliser l'encodeur rotatif afin de naviguer entre les menus. Mais en remarquant le coté aléatoire et le manque de fiabilité de celui-ci nous avons décider de le supprimer de notre montage. Mais pour ne pas se retrouver sans la possibilité de changer de menu, nous avons opté pour le capteur à ultra sons. 
  
  Pour cela nous avons utilisé les composants suivant :
  -Une carte Arduino UNO
  -Une plaquette cuivrée imprimée 
  -Un amplificateur LTC1050
  -Des résistances (2 résistances 100kΩ, 1 résistances 1kΩ, 1 résistance 10kΩ et une résistance 39kΩ)
  -Un potentiomètre digital MCP41050
  -Des capacités (1 capacité 1 μF, x2 capacité 100 nF)
  -Un module Bluetooth HC-06
  -Un écran OLED
  -Un flex-sensor
  -Le capteur de graphite (plusieurs capteurs avec des crayons B et HB)
  -Un capteur à ultra son HC-SR04
  
# 3 - Arduino et son programme :

  Afin de lire les valeurs des résistances avec la carte Arduino nous avons du réaliser un programme qui lis les différentes valeurs de tension aux bornes des capteurs (capteur de graphite et flex-sensor) et les transformer en valeurs de résistance et en angle de flexion. Nous avons choisit de remplacer la résistance fixe R2 par un potentiomètre digital, le programme doit donc pouvoir modifier la valeur de la résistance de celui-ci. Le programme permet de controler l'écran OLED afin d'afficher un menu pour permettre de choisir les données à afficher, que ce soit la valeur de R2 afin de la modifier, d'afficher la valeur du capteur de graphite ou celle du flex sensor. La navigation et la sélection des différents menus se fait à l'aide du capteur à ultra sons, qui mesure la distance avec un objet placé devant.
  
# 4 - Réalisation du PCB sous KiCad :

  KiCad permet de disigner le circuit électrique ainsi que le placement des différents composants et leurs connexions avant l'imprssion de la carte. Le processus se faire en plusieurs étapes : 
  
  Premierement, réaliser les symboles des composants et le shématique du montage. 
  
  ![AOP](https://user-images.githubusercontent.com/123975808/236270953-0dd1b266-4069-4a7a-a7d5-889b840438ef.png)
  ![CapteurUltraSon](https://user-images.githubusercontent.com/123975808/236271023-11e8f291-21aa-4372-a532-135143a22666.png)
  ![OLED](https://user-images.githubusercontent.com/123975808/236271091-735d98da-f3dc-48ab-ae47-e1c56f433704.png)
  ![HC-06](https://user-images.githubusercontent.com/123975808/236271072-e1c05d2c-896e-47fe-b494-ff3b0ff2040c.png)

  Ensuite il a fallu réaliser les empreintes des composants pour le PCB, et le PCB en lui même avec les connexions entre composants (les tailles ont éte imposées pour réduire les erreurs et les problèmes d'impression) : 
  
  ![PCB-BluePrint](https://user-images.githubusercontent.com/123975808/236272249-6de6bcf9-4dca-4102-9818-028ca361b933.png)

# 5 - Fabrication de la carte :

  Une fois le fichier KiCad terminé nous l'avons envoyé à Cathy pour qu'elle puisse nous l'imprimer (un grand merci à elle pour son aide et sa gentilesse d'avoir bien voulu imprimer le PCB une deuxième fois après que notre changement de plan). Une fois la carte imprimer nous avons percer les emplacements pour les pates des composants (1mm pour l'Arduino et 0.8mm pour les composants interieur). S'en est suivi la soudure des composants en suivant la disposition faite sous KiCad. 
  
  ![Montage1](https://user-images.githubusercontent.com/123975808/236276552-98b44f9a-7cd6-4727-ac6d-d53c00d2d67d.jpeg)
  
  
  ![Montage2](https://user-images.githubusercontent.com/123975808/236276579-78c67b95-32c5-4712-8774-69f3a374d1e2.jpeg)
  
# 6 - Banc de Test et Résultats : 

  Comme banc de test nous avons utiliser les cercles concentriques à différents rayons imprimé en 3D : 

  ![BancdeTest](https://user-images.githubusercontent.com/123975808/236404408-0f6d1fc1-4647-4499-9c82-5ccaa70b519a.jpeg)
  
  Nous avons décider de faire les tests avec deux capteur, le premier recouvert avec un crayon B et le second avec un crayon HB. Les test se faisaient en déposant le capteur sur un des cercles du banc, le plaquer pour qu'il épouse la courbure. Ensuite nous avons récupéré grace à la fonction 'Data Stream' d'Excel 30 secondes de données par rayon de courbure, et ce en extension et en compression, avec les deux capteurs. Après ça nous avons fait une moyenne des valeurs de résistances, en retirant les erreurs statistiques visible. Avec ces données et les caractéristiques du capteur nous avons réalisé une [datasheet](https://github.com/MOSH-Insa-Toulouse/2022-2023-4GP-Leonar-Zoe/blob/b27132df8c1d5e796953fb5c742e87faf2610482/Graphite%20Flex%20Sensor.pdf) ce ce capteur. 
  
  ![GraphE](https://user-images.githubusercontent.com/123975808/236405793-955b6743-075a-4e0a-9463-b493ec8aa6d3.png)


  ![GraphC](https://user-images.githubusercontent.com/123975808/236405827-cc78f656-7445-4c81-9d7e-f9d581ce69b4.png)

  
  
