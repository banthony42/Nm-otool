# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    recup_bin.sh                                       :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: banthony <banthony@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/11/28 14:58:09 by banthony          #+#    #+#              #
#    Updated: 2017/12/16 19:56:05 by banthony         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#
#	Recupere le path de tout les fichier de /usr/bin contenant "Mach"
#	Permet de recuperer le path uniquement des fichier contenant des mach-o (fat y compris)

file /usr/bin/* | grep "Mach" | cut -d : -f 1,1 | cut -d '(' -f 1,1 >> usr_bin.txt
cat ./usr_bin.txt | xargs -n 1 otool -hv