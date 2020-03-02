#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import logging
from modules import rconlog_config


logging.basicConfig(format = u'%(levelname)-8s [%(asctime)s] %(message)s',
                    level = logging.WARNING,
                    filename="{BASE_PATH}/logs/rconlog.log"
                    .format(BASE_PATH=rconlog_config.default['BASE_PATH']))