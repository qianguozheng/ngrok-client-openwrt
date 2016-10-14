#
# Copyright (C) 2006-2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ngrok
PKG_VERSION:=20121231
PKG_RELEASE:=1

PKG_SOURCE:=
PKG_SOURCE_URL:=
PKG_MD5SUM:=267783A9929ABA118564400947FE08DC

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/ngrok
	SUBMENU:=HiWeeds
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=yong da applications
	DEPENDS:=+libpthread +libstdcpp +libopenssl 
	URL:=http://xxx.xxx.xxx
endef

define Package/ngrok/description
	The private applications of yong da.
endef

define Build/Prepare
	echo "@@@@ ngrok @@@"
	echo PKG_BUILD_DIR = $(PKG_BUILD_DIR)
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

define Build/Compile
	$(call Build/Compile/Default)
endef

define Package/ngrok/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/etc/init.d
	echo INSTALL_DIR=$(INSTALL_DIR)
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ngrokc $(1)/usr/sbin/
	#$(INSTALL_BIN) ./files/ngrok.init $(1)/etc/init.d/ngrok
	$(INSTALL_BIN) ./files/ngrok.sh $(1)/usr/sbin/ngrok.sh
endef

$(eval $(call BuildPackage,ngrok))
