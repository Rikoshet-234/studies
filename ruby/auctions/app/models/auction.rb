class Auction < ActiveRecord::Base
  attr_accessible :content, :title, :tag_list, :finish_date
  belongs_to :user
  has_many :taggings
  has_many :tags, through: :taggings
  has_many :bids, dependent: :destroy

  validates :user_id,
    presence: true

  validates :title,
    presence: true,
    length: { minimum: 3, maximum: 100 }

  validates :content,
    presence: true

  validates :finish_date,
    presence: true

  default_scope order: 'auctions.created_at DESC'

  def active?(date = Time.now)
    date <= finish_date
  end

  def tag_list
    tags.map(&:name).join(', ')
  end

  def tag_list=(names)
    self.tags = names.split(',').map do |name|
      Tag.where(name: name.strip.downcase).first_or_create
    end
  end

  def days_assoc
    (1..7).map { |d| ["#{d} days", (created_at || Time.now) + d.days] }
  end

end
