"""
Module defining data classes (Pump, Valve, Tank, etc.)
and converting JSON data to objects ready to write to Modbus.
"""

import re

# ---------------- BASE CLASS ---------------------
class BaseData:
    """
    Base class for all water objects.

    Attributes:
        universal_keys (list): Universal attributes (zone, id)
        type_name (str): Type name of the object
    """
    universal_keys = ["zone", "id"]
    type_name = None

    def __init__(self, data: dict):
        """
        Sets type-specific attributes based on JSON data.

        Parameters:
            data (dict): JSON data for this object
        """
        for key in self.expected_keys():
            value = data.get(key, None)
            setattr(self, key, self.round_value(value))

    def expected_keys(self):
        """Abstract method. Must be overridden in subclasses."""
        return []

    def round_value(self, value):
        """
        Round numeric values to 2 decimals, multiply by 100.
        Leaves other types unchanged.

        Parameters:
            value: Value to process
        Returns:
            int, float, or original value
        """
        if isinstance(value, (float, int)):
            if value < 0:
                return 0
        if isinstance(value, float):
            value = round(value, 2)
            value = int(100 * value)
        return value

    def attributes(self):
        """
        Get list of attributes ready for Modbus registers.

        Returns:
            list: Attributes
        """
        att = []
        for key in self.universal_keys + self.expected_keys():
            value = getattr(self, key)
            att.append(value)
        return att

    def __repr__(self):
        all_keys = self.universal_keys + self.expected_keys()
        attrs = ", ".join(f"{k}={getattr(self, k)}" for k in self.expected_keys())
        return f"{self.__class__.__name__}({attrs})"


# ---------------- SUBCLASSES ---------------------
class Pump(BaseData):
    register = 0
    def expected_keys(self):
        return ["status", "flow", "speed", "quality"]


class Valve(BaseData):
    register = 20
    def expected_keys(self):
        return ["status", "flow", "quality"]


class Junction(BaseData):
    register = 40
    def expected_keys(self):
        return ["demand", "quality", "pressure"]


class Tank(BaseData):
    register = 60
    def expected_keys(self):
        return ["level", "quality", "pressure"]


class Pipe(BaseData):
    register = 80
    def expected_keys(self):
        return ["flow", "quality"]


# ---------------- FACTORY FUNCTIONS ---------------------
def create_object_from_data(data: dict):
    """
    Converts JSON data to the correct object type.

    Parameters:
        data (dict): JSON data containing a 'type' key

    Returns:
        BaseData: Object of the correct type
    """
    type_map = {
        "PUMP": Pump,
        "TCV": Valve,
        "JUNCTION": Junction,
        "TANK": Tank,
        "PIPE": Pipe,
        "RESERVOIR": None,
    }

    type_name = data.get("type")
    if not type_name:
        raise ValueError("JSON is missing 'type' attribute")

    cls = type_map.get(type_name)
    if cls is None:
        return None
    cls.type_name = type_name
    return cls(data)


def create_objects(json_string: dict):
    """
    Converts a dictionary of JSON objects into a list of BaseData objects.

    Parameters:
        json_string (dict): Dictionary of JSON objects

    Returns:
        list: List of objects ready for Modbus
    """
    if not isinstance(json_string, dict):
        raise ValueError("JSON must be a dictionary of objects")

    objects = []
    pattern = r'^([a-zA-Z]+)(\d+)-(pipe|pump|valve|house|junction|tank)(\d+)$'

    for outer_key, inner_dict in json_string.items():
        match = re.search(pattern, outer_key)

        if match is None or match.group(3) == "house" and int(match.group(4)) > 4: # Filter houses out if id is bigger than 4 (There are more than 40+ houses in Epanet)
            continue
                
        obj = create_object_from_data(inner_dict)
        if obj is None:
            continue

        if match:
            obj.zone = int(match.group(2))
            obj.id = int(match.group(4))
        objects.append(obj)

    return objects
