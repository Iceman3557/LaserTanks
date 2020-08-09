from models.game import Powerup

class PlayerModel:
    def __init__(self, name: str):
        self.__build(name)

    def __init__(self, name: str, **kwargs):
        '''Build the model using kwargs.
        Values default to none.
        :param **kwargs: The list of class values that are set. (e.g.: deaths=5 sets deaths to 5).
        List of used kwargs: kills, deaths, powerups, robotId, playerId'''

        self.__build(name, kwargs)

    def __build(self, name:str, **kwargs):
        '''Build the model using kwargs.
        Values default to none.
        :param name: The player's name
        :param **kwargs: The list of class values that are set. (e.g.: deaths=5 sets deaths to 5).
        List of used kwargs: kills, deaths, powerups, robotId, playerId'''

        self.name : str = name
        self.kills : int = kwargs.pop('kills')
        self.lives : int = kwargs.pop('lives')
        self.deaths : int = kwargs.pop('deaths')
        self.powerups : list(Powerup) = list(kwargs.pop('powerups'))
        self.timeDied : float = None
        self.__robotId : int = kwargs.pop('robotId')
        self.__playerId : int = kwargs.pop('playerId')

        if self._playerId == None:
            self.__generatePlayerId()

    def __generatePlayerId(self):
        self.__playerId = 0

    def shoot(self, player):
        """This player shoots the player passed as an input.

        Returns: This players number of kills including this
        one
        :param player: The player who was shoot by this one"""
        self.kills += 1
        player.deaths += 1
        return self.kills
    
    def robotIsPlayer(self, robotId):
        '''Returns true if this player has a matching robot
        Id
        :param robotId: The robot id to check'''
        return self._robotId == robotId

    def generatePlayerHtml(self):
        return '''<td>
                    <h3>{self.name}</h3>
                    <h4>Score: <b>{self.score()}</b></h4>
                    <b>Kills: </b>{self.kills}
                    <b>Deaths: </b>{self.deaths}
                    <h3>Power Ups:</h3>
                        {self.listPowerups()}
                </td>'''
    
    def score(self):
        score = 2 * (self.kills + 1) - self.deaths
        return score
    
    def listPowerups(self):
        out = ""

        for ii in self.powerups:
            out = out + '<p>{ii.name}</p>'
        
        return out

