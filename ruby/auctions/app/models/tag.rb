class Tag < ActiveRecord::Base
  attr_accessible :name
  has_many :taggings
  has_many :auctions, through: :taggings

  validates :name,
    presence: true
end
