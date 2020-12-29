# -*- coding: utf-8 -*-
import csdefine

from SpaceContainer import SpaceContainerNomal
from SpaceContainer import SpaceContainerPerson
from SpaceContainer import SpaceContainerTeam
from SpaceContainer import SpaceContainerTong

g_spaceContainerObject = {}
g_spaceContainerObject[ csdefine.SPACE_BELONG_NORMAL ] = SpaceContainerNomal.SpaceContainerNomal
g_spaceContainerObject[ csdefine.SPACE_BELONG_PERSON ] = SpaceContainerPerson.SpaceContainerPerson
g_spaceContainerObject[ csdefine.SPACE_BELONG_TEAM ] = SpaceContainerTeam.SpaceContainerTeam
g_spaceContainerObject[ csdefine.SPACE_BELONG_TONG ] = SpaceContainerTong.SpaceContainerTong


def getContainer( type ):
	return g_spaceContainerObject.get( type, None )

def getPickContainerData( belongType, spaceScriptID, playerEntity, args ):
	return g_spaceContainerObject[ belongType ].packDataToFindSpace( spaceScriptID, playerEntity, args )

def getPickContainerLoginData( belongType, spaceScriptID, playerEntity, args ):
	return g_spaceContainerObject[ belongType ].packDataToLogin( spaceScriptID, playerEntity, args )