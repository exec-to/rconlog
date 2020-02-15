#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import logging
from modules import rconlog_config


logging.basicConfig(format = u'%(levelname)-8s [%(asctime)s] %(message)s',
                    level = logging.DEBUG,
                    filename="%s/logs/rconlog.log" % rconlog_config.default['BASE_PATH'])